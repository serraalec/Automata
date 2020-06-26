/* ========================================================================
   $File: base.h$
   $Date: 5/22/2020$
   $Revision: 1$
   $Creator: Alec Serra $
    ======================================================================== */
#if TODOS
fuck me mate
#endif

#ifndef BASE_H
#define BASE_H
#include "types.h"
#include "matrix.h"
#include "software_render.h"
#include <stdlib.h>

enum State {alive, dead};
struct Automata
{
	Vec_2 location;
	int age;
	State current_state;
	State next_state;
};
struct Set
{
	Vec_2 dimensions;
	Vec_2 scale_factor;
	R32 scale;
	Automata** a;
};

struct Queue 
{ 
    int Front;
	int Rear;
	int Size; 
    int Capacity; 
    Automata** Data; 
};

struct View
{
	void* foo;
};

struct Program_State
{
	bool InitalRun;
	Vec_2 camera_position;
	int counter;
	int sim_speed;
	bool pause;
	
};


struct button_state
{
    int HalfTransitionCount;
    bool32 EndedDown;
};

struct Mouse_Input
{
	Z32 MousePosition[2];
	struct
	{
		button_state Left_Mouse;
		button_state Right_Mouse;
		button_state Middle_Mouse;
	};
};

struct Keyboard_Input
{
	button_state Buttons[7];
	struct
	{
		button_state Esc;
		button_state Alt;
		button_state Tab;
		button_state Control;
		button_state Shift;
		button_state _0;
		button_state _1;
		button_state _2;
		button_state _3;
		button_state _4;
		button_state _5;
		button_state _6;
		button_state _7;
		button_state _8;
		button_state _9;
		button_state A;
		button_state B;
		button_state C;
		button_state D;
		button_state E;
		button_state F;
		button_state G;
		button_state H;
		button_state I;
		button_state J;
		button_state K;
		button_state L;
		button_state M;
		button_state N;
		button_state O;
		button_state P;
		button_state Q;
		button_state R;
		button_state S;
		button_state T;
		button_state U;
		button_state V;
		button_state W;
		button_state X;
		button_state Y;
		button_state Z;
		button_state f1;
		button_state f2;
		button_state f3;
		button_state f4;
		button_state f5;
		button_state f6;
		button_state f7;
		button_state f8;
		button_state f9;
		button_state f10;
		button_state f11;
		button_state f12;
	};
};

struct User_Input
{
	Keyboard_Input Keyboard;
	Mouse_Input Mouse;
};
inline Vec_2 RotatePosition(Vec_2 PositionToRotate,
							Vec_2 OriginOfRotation, 
							R32 Angle);
void UpdateTransform(Program_State *State,
					Vec_2 WindowInfo);
void UpdateAndRender(Program_State *State,
					RenderBuffer *Buffer,
					User_Input *Input);
Queue* CreateQueue(N32 Cap);
void EnQueue(Queue *Q, Automata *Data);
Automata* DeQueue(Queue* Q);
Automata* GetFront(Queue* Q); 
Automata* GetRear(Queue* Q);				

#endif