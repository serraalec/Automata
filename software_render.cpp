/* ========================================================================
   $File: software_render.cpp$
   $Date: 5/22/2020$
   $Revision: 1$
   $Creator: Alec Serra $
    ======================================================================== */
#if TODOS
//None so far lmao
#endif

#include "software_render.h"

/* New type convention
MY_DEFINE
MyType
my_function()
my_variable
*/

void swap_z32(Z32 *a, Z32 *b)
{
	if(*a != *b)
	{
		*a = *a + *b;
		*b = *a - *b; 
		*a = *a - *b;
	}
}
void swap_r32(R32 *a, R32 *b)
{
	if(*a != *b)
	{
		*a = *a + *b;
		*b = *a - *b; 
		*a = *a - *b;
	}
}
R32 abs_r32(R32 in)
{
	if(in < 0) return -1*in;
	else return in;
}
Z32 abs_z32(Z32 in)
{
	if(in < 0) return -1*in;
	else return in;
}


void draw_wire_triangle_r32(R32 x1, R32 y1,
						  R32 x2, R32 y2, 
						  R32 x3, R32 y3,
						  RenderBuffer *buffer,
						  ColorN32 *color)
{
	draw_line_r32(x1, y1, x2, y2, buffer, color);
	draw_line_r32(x2, y2, x3, y3, buffer, color);
	draw_line_r32(x3, y3, x1, y1, buffer, color);
}
void draw_wire_triangle_Vec_2(Vec_2 a, Vec_2 b, Vec_2 c, RenderBuffer *buffer, ColorN32 *color)
{
	draw_line_r32(a.I, a.J, b.I, b.J, buffer, color);
	draw_line_r32(b.I, b.J, c.I, c.J, buffer, color);
	draw_line_r32(c.I, c.J, a.I, a.J, buffer, color);
}

void draw_line_vec_2(Vec_2 a, Vec_2 b, RenderBuffer* buffer, ColorN32 *color)
{
	draw_line_r32(a.I, a.J, b.I, b.J,
				buffer, color);
}
//Need to make a map from line world to screen coords.
//basic line formula y = mx+b, where b = y1
//But when m > 1 a smooth line is not created.  So,
//an additional check must be made
void draw_line_r32(R32 x1, R32 y1, R32 x2, R32 y2, RenderBuffer* buffer, ColorN32 *color)
{
	if(x1 >= (R32)buffer->Width) x1 = (R32)buffer->Width;
	if(x2 >= (R32)buffer->Width) x2 = (R32)buffer->Width;
	if(y1 >= (R32)buffer->Height) y1 = (R32)buffer->Height;
	if(y2 >= (R32)buffer->Height) y2 = (R32)buffer->Height;
	if(x1 < 0.0f) x1 = 0.0f;
	if(x2 < 0.0f) x2 = 0.0f;	
	if(y1 < 0.0f) y1 = 0.0f;
	if(y2 < 0.0f) y2 = 0.0f;	
	
	R32 dy, dx, m, epsilon;
	dy = y2-y1;
	dx = x2-x1;
	m = dy/dx;
	epsilon = 0;
	
	
	
	#if 0
	
	N8 *endofbuffer = (N8 *)buffer->Memory + buffer->Pitch*buffer->Height;
	N8 *pixel = ((N8 *)buffer->Memory +
        x*buffer->BytesPerPixel +
        y*buffer->Pitch);

	if(pixel <= endofbuffer)
	{
		*(N32 *)pixel = ((color->Red << 16)
						|(color->Green << 8) 
						|color->Blue);;
	}		
	#endif
	
	if((0 <= m && m <= 1) && x1 < x2)
	{
		R32 y = y1;
		for (R32 x = x1; x <= x2; x++)  
		{
			draw_point_r32(x, y, buffer, color);
			epsilon += dy;
			if ((epsilon/2) >= dx )  
			{
				y++;  
				epsilon -= dx;
			}
		}
	}
	else if(0 >= m && m >= -1 && x1 < x2)
	{
		R32 y = y1;
		for (R32 x = x1; x <= x2; x++)  
		{
			draw_point_r32(x, y, buffer, color);
			epsilon += dy;
			if ((epsilon/2) <= dx)  
			{
				y--;  
				epsilon += dx;
			}
		}
	}
	else if((0 >= m && m >= -1) && x2 < x1)
	{
		R32 y = y1;
		for (R32 x = x1; x >= x2; x--)  
		{
			draw_point_r32(x, y, buffer, color);
			epsilon += dy;
			if ((epsilon/2) >= dx )  
			{
				y++;  
				epsilon += dx;
			}
		}
	}
	
	else if((1 < m )&&  y1 < y2)
	{
		R32 x = x1;
		for (R32 y = y1; y <= y2; y++)  
		{
			draw_point_r32(x, y, buffer, color);
			epsilon += dx;
			if ((epsilon) >= dx )  
			{
				x++;  
				epsilon -= dy;
			}
		}	
	}
	else if(y2 < y1 && 1 < m)
	{
		R32 x = x1;
		for (R32 y = y1; y >= y2; y--)  
		{
			draw_point_r32(x, y, buffer, color);
			epsilon -= dx;
			if ((epsilon) >= dx )  
			{
				x--;  
				epsilon += dy;
			}
		}
	}
	else if((0 <= m && m <= 1) &&  x2 < x1) 
	{	
		R32 y = y1;

		for (R32 x = x1; x >= x2; x--)  
		{
			draw_point_r32(x, y, buffer, color);
			epsilon -= dy;
			if ((epsilon/2) >= dx )  
			{
				y--;  
				epsilon += dx;
			}
		}
	}
	else if(m < -1 && y1 < y2)
	{
		R32 x = x1;
		dx = abs_r32(dx);
		dy = abs_r32(dy);
		for (R32 y = y1; y <= y2; y++)  
		{
			draw_point_r32(x, y, buffer, color);
			epsilon += dx;
			if ((epsilon) >= dx )  
			{
				x--;  
				epsilon -= dy;
			}
		}
	}	
	
	else if(y2 < y1 && -1 > m)
	{
		R32 x = x1;
		dx = abs_r32(dx);
		dy = abs_r32(dy);
		for (R32 y = y1; y >= y2; y--)  
		{
			draw_point_r32(x, y, buffer, color);
			epsilon += dx;
			if ((epsilon) >= dx )  
			{
				x++;  
				epsilon -= dy;
			}
		}
	}
}

inline void draw_point_n32(N32 x, N32 y, RenderBuffer* buffer, ColorN32 *color)
{			
	if(x >= buffer->Width) x = buffer->Width;
	if(y >= buffer->Height) y = buffer->Height;
	
	if(x < 0) x = 0;
	if(y < 0) y = 0;
	
	N8 *endofbuffer = (N8 *)buffer->Memory + buffer->Pitch*buffer->Height;
	
	N8 *pixel = ((N8 *)buffer->Memory +
        x*buffer->BytesPerPixel +
        y*buffer->Pitch);

	if(pixel <= endofbuffer)
	{
		*(N32 *)pixel = ((color->Red << 16)
						|(color->Green << 8) 
						|color->Blue);;
	}		
}

inline void draw_point_r32(R32 x, R32 y, RenderBuffer* buffer, ColorN32 *color)
{
	draw_point_n32((int)x,
				   (int)y,
				   buffer,
				   color);
}


void draw_2_tone_vec_2(Vec_2 v1,
									Vec_2 v2,
									RenderBuffer *buffer,
									N64* texture,
									N64 texture_size,
									N64 array_pitch)
{	
	int x1 = (Z32) v1.I;
	int x2 = (Z32) v2.I;
	int y1 = (Z32) v1.J;
	int y2 = (Z32) v2.J;
	
	
	if(x1 > buffer->Width) x1 = buffer->Width;
	if(y1 > buffer->Height) y1 = buffer->Height;
	if(x1 < 0) x1 = 0;
	if(y1 < 0) y1 = 0;
	
	if(x2 > buffer->Width) x2 = buffer->Width;
	if(y2 > buffer->Height) y2 = buffer->Height;
	if(x2 < 0) x2 = 0;
	if(y2 < 0) y2 = 0;
	
	if(x1 > x2)
	{
		swap_z32(&x1, &x2);
	}
	if(y1 > y2)
	{
		swap_z32(&y1, &y2);
	}
	
	N8 *EndOfbuffer = (N8 *)buffer->Memory + buffer->Pitch*buffer->Height;
	N8  bit_counter = 0;
	int array_x_index = 0;
	int array_y_index = 0;
	for(int X = x1;
        X <= x2;
        ++X)
    {
        N8 *Pixel = ((N8 *)buffer->Memory +
                        X*buffer->BytesPerPixel +
                        y1*buffer->Pitch);
        for(int Y = y1;
            Y <= y2;
            ++Y)
        {
            if((Pixel >= buffer->Memory) &&
               ((Pixel + 4) <= EndOfbuffer) &&
			   X*Y <= texture_size)
            {
				//unsigned int so left shift is cooleo hooleo
                *(N32 *)Pixel = (texture[array_y_index*array_pitch + array_x_index] & (1LLU << bit_counter)) >> bit_counter 
					? 0xFFFFFFFF : 0x00000000;
				//	(n & ( 1 << k )) >> k	<- kth bit of n
				bit_counter++;
				if(bit_counter == 64)
				{
					bit_counter = 0;
					array_x_index++;
					if(array_x_index > array_pitch)
					{
						array_x_index = 0;
						array_y_index++;
					}
				}
				
            }

            Pixel += buffer->Pitch;
        }
    }
}


void draw_rect_z32(Z32 x1, Z32 y1, Z32 x2, Z32 y2, RenderBuffer *buffer, ColorN32 *color)
{		
	if(x1 > buffer->Width) x1 = buffer->Width;
	if(y1 > buffer->Height) y1 = buffer->Height;
	if(x1 < 0) x1 = 0;
	if(y1 < 0) y1 = 0;
	
	if(x2 > buffer->Width) x2 = buffer->Width;
	if(y2 > buffer->Height) y2 = buffer->Height;
	if(x2 < 0) x2 = 0;
	if(y2 < 0) y2 = 0;
	
	if(x1 > x2)
	{
		swap_z32(&x1, &x2);
	}
	if(y1 > y2)
	{
		swap_z32(&y1, &y2);
	}
	
	N8 *EndOfbuffer = (N8 *)buffer->Memory + buffer->Pitch*buffer->Height;
	
	for(int X = x1;
        X <= x2;
        ++X)
    {
        N8 *Pixel = ((N8 *)buffer->Memory +
                        X*buffer->BytesPerPixel +
                        y1*buffer->Pitch);
        for(int Y = y1;
            Y <= y2;
            ++Y)
        {
            if((Pixel >= buffer->Memory) &&
               ((Pixel + 4) <= EndOfbuffer))
            {
                *(N32 *)Pixel = ((color->Red << 16)
						|(color->Green << 8) 
						|color->Blue);
            }

            Pixel += buffer->Pitch;
        }
    }
}

void draw_rect_vec_2(Vec_2 v1,
				   Vec_2 v2,
				   RenderBuffer *buffer,
				   ColorN32 *color,
				   void *texture)
{
	draw_rect_z32((Z32)v1.I, (Z32)v1.J,
				  (Z32)v2.I, (Z32)v2.J,
				  buffer, color);
}
