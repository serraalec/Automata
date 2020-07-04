#ifndef SOFTWARE_RENDER_H
#define SOFTWARE_RENDER_H

#include "types.h"
#include "math.h"

struct ColorR32
{
	R32 Red;
	R32 Green;
	R32 Blue;
	R32 Grayscale;
};	

struct ColorN32
{
	N8 Red;
	N8 Green;
	N8 Blue;
	N8 Grayscale;
};

struct RenderBuffer
{
	N16 Width;
    N16 Height;
    N16 Pitch;
	Z64 BytesPerPixel;
	void * Memory;
};
struct Quad
{
	N32 Width;
	N32 Height;
	void *Texture;
	ColorN32 Color;
};

struct Triangle
{
	Vec_3 A;
	Vec_3 B;
	Vec_3 C;
};
struct Mesh
{
	Triangle* Triangles;
	Vec_3 Position;
};




void swap_z32(Z32 *a, Z32 *b);
void swap_r32(R32 *a, R32 *b);
R32 abs_r32(R32 in);
Z32 abs_z32(Z32 in);
void draw_wire_triangle_r32(R32 x1, R32 y1,R32 x2, R32 y2, R32 x3, R32 y3, RenderBuffer *buffer, ColorN32 *color);
void draw_wire_triangle_Vec_2(Vec_2 a, Vec_2 b, Vec_2 c, RenderBuffer *buffer, ColorN32 *color);
void draw_line_vec_2(Vec_2 a, Vec_2 b, RenderBuffer* buffer, ColorN32 *color);
void draw_line_r32(R32 x1, R32 y1, R32 x2, R32 y2, RenderBuffer* buffer, ColorN32 *color);
inline void draw_point_n32(N32 x, N32 y, RenderBuffer* buffer, ColorN32 *color);
inline void draw_point_r32(R32 x, R32 y, RenderBuffer* buffer, ColorN32 *color);
void draw_rect_z32(Z32 x1, Z32 y1, Z32 x2, Z32 y2, RenderBuffer *buffer, ColorN32 *color);
void draw_rect_vec_2(Vec_2 v1, Vec_2 v2, RenderBuffer *buffer, ColorN32 *color, void *texture);
void draw_2_tone_vec_2(Vec_2 v1,
									Vec_2 v2,
									RenderBuffer *buffer,
									N64* texture,
									N64 texture_size,
									N64 array_pitch);

#endif