/* ========================================================================
   $File: win32_base.h$
   $Date: 5/22/2020$
   $Revision: 1$
   $Creator: Alec Serra $
    ======================================================================== */

#if TODOS
fuck me mate
//#include <dsound.h> //<- Get this working
//#include <stdint.h>// <-- and this
#endif

#ifndef WIN32_BASE_H
#define WIN32_BASE_H

#include <windows.h>  
#include <Windowsx.h>
#include <XInput.h>
#include <dsound.h> 
#include <stdio.h>
#include <stdint.h>

#include "types.h"
#include "base.cpp"
#include "software_render.cpp"

struct FrameRateInformation
{
	R32 RefreshRate; //[Hz]
	N64 FirstTick;
	N64 LastTick;
	N64 ElapsedTicks;
	N64 Frequency;
	N64 ElapsedMillSeconds;
	N64 GoalMillSeconds;
};

internal void
Win32ProcessPendingMessages(User_Input &Input);
void UpdateFramerateInfo(FrameRateInformation FrameRate);
internal void
Win32ProcessKeyboardMessage(button_state *NewState, bool32 IsDown);
struct WindowDim {
    N16 Width;
    N16 Height;
};

struct Program_Memory
{
    N64 PersistentStorageSize; 
	void *PersistentStorage; //
    N64 TransientStorageSize;
	void *TransientStorage; //
    bool32 isInited;
	N64 TransientFreeLocation;
	N64 PersistentFreeLocation;
};

struct Offscreen_Buffer
{
    BITMAPINFO Info;
    void * Memory;
    N16 Width;
    N16 Height;
    N16 Pitch;
	Z64 BytesPerPixel;
};



void UpdateFramerateInfo(FrameRateInformation *Framerate);

void UpdateTransform(Program_State *State,
					WindowDim *WindowInfo);
R32 Absolute_Value(R32 in);

int Absolute_Value(int in);
				
void UpdateAndRener(Program_State *State);

void DisplayBuffer(HDC Device,
                   Offscreen_Buffer Buffer);
void SDLResizeTexture(Offscreen_Buffer *Buffer, int Width, int Height);

void * PlatformLoadFile(char *FileName);

void ResizeDIBSection(Offscreen_Buffer *Buffer,
					  Program_Memory &Memory,
						N16 Width, N16 Height);
int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CommandLine,
        int ShowCode);
LRESULT CALLBACK Win32MainWindowCallback(
HWND hWnd, 
UINT Message, 
WPARAM wParam, 
LPARAM lParam);
void DisplayError(HWND WindowHandle);

void AllocateMemory(Program_Memory &Mem,
					N64 TransientStorageSize,
					N64 PersistentStorageSize);
		
#endif