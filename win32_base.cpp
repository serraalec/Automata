/* ========================================================================
   $File: base.cpp$
   $Date: 5/22/2020$
   $Revision: 1$
   $Creator: Alec Serra $
    ======================================================================== */
#if TODOS
fuck me mate
//TODO: Add File support
//TODO: Get rid of globals
//TODO: Add Sound support
//TODO: Add logging of FrameRate oopsies
#endif
#include "win32_base.h"

global_variable int ErrorCode;
global_variable bool Running;
global_variable WindowDim WindowDimension;
global_variable Offscreen_Buffer GlobalBackbuffer;
global_variable Program_Memory Memory;
UINT DesiredSchedulerMilliseconds = 1;
bool SleepIsGranular = timeBeginPeriod(DesiredSchedulerMilliseconds) == TIMERR_NOERROR;

inline N64 GetPerformanceCount()
{
	LARGE_INTEGER Count;
	QueryPerformanceCounter(&Count);
	
	return (Count.QuadPart);
}

inline N64 GetPerformanceFrequency()
{
	LARGE_INTEGER Freq;
	QueryPerformanceFrequency(&Freq);
	
	return (Freq.QuadPart);
}

int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CommandLine,
		int nCmdShow)
{
	WNDCLASS WindowClass = {};
    WindowDimension = {1280, 720};
	ErrorCode = 0;
	GlobalBackbuffer = {};
	User_Input Input = {};
	Memory = {};
	Program_State State = {};
	State.InitalRun = true;
	FrameRateInformation FrameRate = {};
	FrameRate.RefreshRate = 30;
	Running = true;
	
	// Allocate Game Memory
	AllocateMemory(Memory,
				   Megabytes(10),
				   Megabytes(10));
	
	
	
    WindowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.hIcon = LoadIcon(NULL, IDI_QUESTION); 
    WindowClass.lpszClassName = "B_Base";    
	
	ResizeDIBSection(&GlobalBackbuffer,
					  Memory,
					  WindowDimension.Width,
					  WindowDimension.Height);
				
    if(RegisterClass(&WindowClass))
    {
        HWND WindowHandle =
            CreateWindowEx(
            0,
            WindowClass.lpszClassName,
            "Base",
            WS_OVERLAPPEDWINDOW|WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
			WindowDimension.Width,
            WindowDimension.Height,
            NULL,
            NULL,
            Instance,
            NULL);
        
        if(WindowHandle)
        {
            HDC DeviceContext = GetDC(WindowHandle);
            N8 XOffset = 0;
            N8 YOffset = 0;
            
            while(Running)
            {
                MSG Message;
                
				
				FrameRate.Frequency = GetPerformanceFrequency(); 
				FrameRate.FirstTick = GetPerformanceCount();
                
				Win32ProcessPendingMessages(Input);
				
				RenderBuffer buffer = {};
				buffer.Width = GlobalBackbuffer.Width;
				buffer.Height = GlobalBackbuffer.Height;
				buffer.Pitch = GlobalBackbuffer.Pitch;
				buffer.BytesPerPixel = GlobalBackbuffer.BytesPerPixel;
				buffer.Memory = GlobalBackbuffer.Memory;
				
				
				UpdateAndRender(&State,
								&buffer,
								&Input);
		
                DisplayBuffer(DeviceContext, GlobalBackbuffer);
				if(ErrorCode)
				{
					void DisplayError(HWND WindowHandle);
				}

				FrameRate.LastTick = GetPerformanceCount();				
				UpdateFramerateInfo(FrameRate);
			}
        }
        else
        {
			assert(0 == 1);
		}
    }
    else
    {
		assert(0 == 1);
	}
    return(0);
}

void UpdateFramerateInfo(FrameRateInformation FrameRate)
{
	FrameRate.ElapsedTicks = (FrameRate.LastTick - FrameRate.FirstTick);
	
	FrameRate.ElapsedMillSeconds = (FrameRate.ElapsedTicks*1000)/FrameRate.Frequency;
	FrameRate.GoalMillSeconds = 1000/(N64)FrameRate.RefreshRate;

    if(FrameRate.ElapsedMillSeconds <= FrameRate.GoalMillSeconds)
    {                        
        if(SleepIsGranular)
        {
			
			DWORD SleepMS = (DWORD)((FrameRate.GoalMillSeconds - FrameRate.ElapsedMillSeconds));
            if(SleepMS > 0)
            {
                Sleep(SleepMS);
            }
		
        }
	}
	else
	{
#if 1
		//TODO framerate miss logging.
		char buffer[100];
		sprintf_s(buffer, "\n Missed target framerate total ticks %llu \n ", FrameRate.ElapsedTicks);
		OutputDebugStringA(buffer);	
#endif
	}	
}

void AllocateMemory(Program_Memory &Mem,
					N64 TransientStorageSize,
					N64 PersistentStorageSize)
{
	Mem.TransientStorageSize = Megabytes(64);
	Mem.PersistentStorageSize = Megabytes(64);
	
	LPVOID AddressToAllocate = NULL;//0x2A037010000;
	
	Mem.TransientStorage = VirtualAlloc(AddressToAllocate, Mem.TransientStorageSize, MEM_COMMIT, PAGE_READWRITE);
	
	//AddressToAllocate = 0x2A03B010000;
	
	Mem.PersistentStorage = VirtualAlloc(0, Mem.PersistentStorageSize, MEM_COMMIT, PAGE_READWRITE);
	
	Mem.TransientFreeLocation = (N64)Mem.TransientStorage;
	
	Mem.isInited = true;
	#if TODOS
	Fix mem allocation
	if(!Mem.TransientStorage || Mem.PersistentStorage)
	{
		ErrorCode = 2;
		assert(1 == 0);
	}		
	#endif
}

internal void
Win32ProcessKeyboardMessage(button_state *NewState, bool32 IsDown)
{
    if(NewState->EndedDown != IsDown)
    {
        NewState->EndedDown = IsDown;
        ++NewState->HalfTransitionCount;
    }
}

internal void
Win32ProcessPendingMessages(User_Input &Input)
{
    MSG Message;
    while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
    {
        switch(Message.message)
        {		
			case WM_QUIT:
			{
				Running = false;
			}
			case WM_MOUSELAST:
			case WM_MOUSEMOVE:
			{
				Input.Mouse.MousePosition[0] = GET_X_LPARAM(Message.lParam); 
				Input.Mouse.MousePosition[1] = GET_Y_LPARAM(Message.lParam);
			} break;

			case WM_LBUTTONDOWN:
			{
				N32 VKCode = (N32)Message.wParam;
				// NOTE(casey): Since we are comparing WasDown to IsDown,
				// we MUST use == and != to convert these bit tests to actual
				// 0 or 1 values.
				bool32 WasDown = ((Message.lParam & (1 << 30)) != 0);
				bool32 IsDown = ((Message.lParam & (1 << 31)) == 0);
				if(WasDown != IsDown)	Win32ProcessKeyboardMessage(&Input.Mouse.Left_Mouse, IsDown);
			} break;
			
			case WM_LBUTTONUP:
			{
				N32 VKCode = (N32)Message.wParam;
				// NOTE(casey): Since we are comparing WasDown to IsDown,
				// we MUST use == and != to convert these bit tests to actual
				// 0 or 1 values.
				bool32 WasDown = ((Message.lParam & (1 << 30)) != 0);
				bool32 IsDown = ((Message.lParam & (1 << 31)) == 0);
				if(WasDown != IsDown)	Win32ProcessKeyboardMessage(&Input.Mouse.Left_Mouse, IsDown);
			} break;
			
			case WM_MOUSEWHEEL:
			{
				
			} break;
			
			case WM_KEYUP:
			case WM_KEYDOWN:
			{
				N32 VKCode = (N32)Message.wParam;
				// NOTE(casey): Since we are comparing WasDown to IsDown,
				// we MUST use == and != to convert these bit tests to actual
				// 0 or 1 values.
				bool32 WasDown = ((Message.lParam & (1 << 30)) != 0);
				bool32 IsDown = ((Message.lParam & (1 << 31)) == 0);
				if(WasDown != IsDown)
				{
					switch(VKCode)
					{
						//This is fucking retarded
						case 0x30: { Win32ProcessKeyboardMessage(&Input.Keyboard._0, IsDown); } break;  //0 key
						case 0x31: { Win32ProcessKeyboardMessage(&Input.Keyboard._1, IsDown); } break;  //1 key
						case 0x32: { Win32ProcessKeyboardMessage(&Input.Keyboard._2, IsDown); } break;  //2 key
						case 0x33: { Win32ProcessKeyboardMessage(&Input.Keyboard._3, IsDown); } break;  //3 key
						case 0x34: { Win32ProcessKeyboardMessage(&Input.Keyboard._4, IsDown); } break;  //4 key
						case 0x35: { Win32ProcessKeyboardMessage(&Input.Keyboard._5, IsDown); } break;  //5 key
						case 0x36: { Win32ProcessKeyboardMessage(&Input.Keyboard._6, IsDown); } break;  //6 key
						case 0x37: { Win32ProcessKeyboardMessage(&Input.Keyboard._7, IsDown); } break;  //7 key
						case 0x38: { Win32ProcessKeyboardMessage(&Input.Keyboard._8, IsDown); } break;  //8 key
						case 0x39: { Win32ProcessKeyboardMessage(&Input.Keyboard._9, IsDown); } break;  //9 key
						case 0x41: { Win32ProcessKeyboardMessage(&Input.Keyboard.A, IsDown);  } break;  //A key
						case 0x42: { Win32ProcessKeyboardMessage(&Input.Keyboard.B, IsDown);  } break;  //B key
						case 0x43: { Win32ProcessKeyboardMessage(&Input.Keyboard.C, IsDown);  } break;  //C key
						case 0x44: { Win32ProcessKeyboardMessage(&Input.Keyboard.D, IsDown);  } break;  //D key 
						case 0x45: { Win32ProcessKeyboardMessage(&Input.Keyboard.E, IsDown);  } break;  //E key
						case 0x46: { Win32ProcessKeyboardMessage(&Input.Keyboard.F, IsDown);  } break;  //F key
						case 0x47: { Win32ProcessKeyboardMessage(&Input.Keyboard.G, IsDown);  } break;  //G key 
						case 0x48: { Win32ProcessKeyboardMessage(&Input.Keyboard.H, IsDown);  } break;  //H key
						case 0x49: { Win32ProcessKeyboardMessage(&Input.Keyboard.I, IsDown);  } break;  //I key
						case 0x4A: { Win32ProcessKeyboardMessage(&Input.Keyboard.J, IsDown);  } break;  //J key
						case 0x4B: { Win32ProcessKeyboardMessage(&Input.Keyboard.K, IsDown);  } break;  //K key
						case 0x4C: { Win32ProcessKeyboardMessage(&Input.Keyboard.L, IsDown);  } break;  //L key 
						case 0x4D: { Win32ProcessKeyboardMessage(&Input.Keyboard.M, IsDown);  } break;  //M key 
						case 0x4E: { Win32ProcessKeyboardMessage(&Input.Keyboard.N, IsDown);  } break;  //N key
						case 0x4F: { Win32ProcessKeyboardMessage(&Input.Keyboard.O, IsDown);  } break;  //O key 
						case 0x50: { Win32ProcessKeyboardMessage(&Input.Keyboard.P, IsDown);  } break;  //P key
						case 0x51: { Win32ProcessKeyboardMessage(&Input.Keyboard.Q, IsDown);  } break;  //Q key
						case 0x52: { Win32ProcessKeyboardMessage(&Input.Keyboard.R, IsDown);  } break;  //R key 
						case 0x53: { Win32ProcessKeyboardMessage(&Input.Keyboard.S, IsDown);  } break;  //S key
						case 0x54: { Win32ProcessKeyboardMessage(&Input.Keyboard.T, IsDown);  } break;  //T key
						case 0x55: { Win32ProcessKeyboardMessage(&Input.Keyboard.U, IsDown);  } break;  //U key
						case 0x56: { Win32ProcessKeyboardMessage(&Input.Keyboard.V, IsDown);  } break;  //V key
						case 0x57: { Win32ProcessKeyboardMessage(&Input.Keyboard.W, IsDown);  } break;  //W key
						case 0x58: { Win32ProcessKeyboardMessage(&Input.Keyboard.X, IsDown);  } break;  //X key
						case 0x59: { Win32ProcessKeyboardMessage(&Input.Keyboard.Y, IsDown);  } break;  //Y key
						case 0x5A: { Win32ProcessKeyboardMessage(&Input.Keyboard.Z, IsDown);  } break;  //Z key
						
						case VK_CONTROL: { Win32ProcessKeyboardMessage(&Input.Keyboard.Control, IsDown); } break;
						case VK_SHIFT: { Win32ProcessKeyboardMessage(&Input.Keyboard.Shift, IsDown); } break;
						case VK_TAB: { Win32ProcessKeyboardMessage(&Input.Keyboard.Tab, IsDown);} break;
						case VK_ESCAPE: { Running = false; } break;
						
					}
				}
			}
			default:
            {
                TranslateMessage(&Message);
                DispatchMessageA(&Message);
            } break;
		
		}
	}
}

LRESULT CALLBACK Win32MainWindowCallback(
HWND hWnd, 
UINT Message, 
WPARAM wParam, 
LPARAM lParam)  
{  
    LRESULT Result = 0;
    RECT  lpRect;
    switch(Message)
    {
        case WM_CLOSE:
        {
        } break;
        
        case WM_SETCURSOR:
        {
            
        } break;
		
        case WM_MOVING:
        case WM_MOVE: 
        case WM_SIZING:
        case WM_SIZE:
        {
            if(GetWindowRect(hWnd, &lpRect))
            {
                
                WindowDimension.Width = (N16)(lpRect.right -  lpRect.left);
                WindowDimension.Height = (N16)(lpRect.bottom - lpRect.top);
				
				ResizeDIBSection(&GlobalBackbuffer,
								  Memory,
								  WindowDimension.Width,
								  WindowDimension.Height);
            }//Shits fucked yo
            else
            {
                ErrorCode = 1;
				assert(1 == 0);
            }

        }break;
        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(hWnd, &Paint);
            DisplayBuffer(DeviceContext,GlobalBackbuffer);
            EndPaint(hWnd, &Paint);
        } break;
        
        default:
        {
            Result = 0;
        } break;
    }
    
    return DefWindowProc(hWnd, Message, wParam, lParam);
} 



void DisplayBuffer(HDC Device,
                   Offscreen_Buffer Buffer)
{
    
    StretchDIBits(
        Device,
        0,0,WindowDimension.Width, WindowDimension.Height,
        0,0,Buffer.Width, Buffer.Height,
        Buffer.Memory,
        &Buffer.Info,
        DIB_RGB_COLORS,
        SRCCOPY);
}
void ResizeDIBSection(Offscreen_Buffer *Buffer,
					  Program_Memory &Mem,
						N16 Width, N16 Height)
{
	if(Buffer->Memory != NULL)
	{
		Mem.TransientFreeLocation = (N64)Mem.TransientStorage;
	}
	
    Buffer->Width = Width;
    Buffer->Height = Height;
    
    Buffer->BytesPerPixel = 4;
    
    
    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height;
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biCompression = BI_RGB;
    
    N64 BitmapMemorySize = (Buffer->Width*Buffer->Height)*Buffer->BytesPerPixel;
    Buffer->Memory = (N64*)Mem.TransientFreeLocation;
	Mem.TransientFreeLocation +=BitmapMemorySize;
    Buffer->Pitch =(N16)(Width*Buffer->BytesPerPixel);
}

void DisplayError(HWND WindowHandle)
{
	LPCTSTR text = "Uh oh Stinky";
	switch(ErrorCode)
	{
        case 1:
        {
			text = "GetWindowRect Fail";
        } break;
        
        case 2:
        {
            text = "Mem Allocate Fail";
        } break;
        //Keyboard Events
        case 3:
        {
			text = "Wa Wa";
        }break;
        case 4:
        case 5: 
        case 6:
        case 7:
        {
			text = "Wa Wa";
		}break;
		default:
		{
			
		}break;
	}
		
	MessageBox(WindowHandle,
						text,
						NULL,
						MB_OK);	
}



