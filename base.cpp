/* ========================================================================
   $File: bashe.cpp$
   $Date: 5/24/2020$
   $Revision: 1$
   $Creator: Alec Serra $
    ======================================================================== */

#include "base.h"
#include "matrix.cpp"

#define LENGTH 100

/*

TODO: Fix this stillborn nightmare.


*/


/*	//RULES//
	Any live cell with two or three live neighbours survives.
	Any dead cell with three live neighbours becomes a live cell.
	All other live cells die in the next generation. Similarly, all other dead cells stay dead.	*/

//REAL Slooooooooooooooooooooooooooooow.
//Dynamically allocate an array of size N64 and map that to each cell.

int Init_Set(Set *s)
{	

	s->current_state = (N64*)	calloc(((LENGTH/64+1)*(LENGTH/64+1)), sizeof(N64));
	s->next_state = 	(N64*)		calloc(((LENGTH/64+1)*(LENGTH/64+1)), sizeof(N64));
	
	for(int y_index = 0; y_index < LENGTH/64; y_index++)
	{
		for(int x_index = 0; x_index < LENGTH/64; x_index++)
		{
			s->current_state[x_index + LENGTH/64*y_index] = 0x0;
			s->next_state[x_index + LENGTH/64*y_index] = 0x0;
		}
	}
	
	
	/*
	number |= 1 << x; <- set a bit
	number &= ~(1 << x); <-clear a bit
	number ^= 1 << x; <- toggle a bit
	(n & ( 1 << k )) >> k	<- kth bit of n
	*/
	//init game
	
	return (0);
}
int Render_Backplane(Set *s, Vec_2 camera_position, RenderBuffer *buffer)
{

	return (0);
}

int Render_Automata(Set *s, Vec_2 camera_position, RenderBuffer *buffer)
{
		return (0);
}

void UpdateAndRender(Program_State *state, RenderBuffer *buffer, User_Input *input)
{
	local_persist Set s;
	ColorN32 c = {};
	
	if(state->InitalRun)
	{
		state->InitalRun = false;
		
		state->counter = 0;
		state->sim_speed = 0;
		state->pause = 0;
		state->camera_position = {};
		//Init the system to be dead and setup location array for all elements.
		Init_Set(&s);
	}


	
	//Clear Screen	
	state->counter++;
	c.Red = 255;	
	/*
	draw_rect_z32(0, 0,
				buffer->Width, buffer->Height,
				buffer, &c);
	*/
	int neighbor_count = 0;
	for(int y_index = 0; y_index < (LENGTH/64+1); y_index++)
	{
		for(int x_index = 0; x_index < (LENGTH/64+1); x_index++)
		{
			for(N64  bitwise = 0; bitwise < 64; bitwise++)
			{	
				s.current_state[x_index + LENGTH/64*y_index] &= 1LLU >> bitwise; 
			}
		}
	}
	
	//draw_rect_z32(0, 0, buffer->Width, buffer->Height, buffer, &c);
	
	Vec_2 p1 = {};
	Vec_2 p2 = {(R32)LENGTH, (R32)LENGTH};
	draw_2_tone_vec_2(p1, p2,
								buffer, s.current_state,
								LENGTH*LENGTH,
								LENGTH/64);
	//Run through a loop and apply the bitwise ops

	/*
	if(input->Keyboard.Shift.EndedDown)	s.scale += .01f;
	if(input->Keyboard.Control.EndedDown)	s.scale -= .01f;
	
	Vec_2 delta = {};
	if(input->Keyboard.W.EndedDown) delta.J += 1.0f;
	if(input->Keyboard.D.EndedDown) 
		delta.I -= 1.0f;
	if(input->Keyboard.S.EndedDown) delta.J -= 1.0f;
	if(input->Keyboard.A.EndedDown) delta.I += 1.0f;
	
	if(input->Keyboard.C.EndedDown) state->pause = !state->pause;
	
		norm_vec_2(delta);
	mult_r32_vec_2(1.0f/s.scale * 10.0f, delta);
	add_vec_2(delta, state->camera_position, state->camera_position);
	
	
	if(input->Keyboard.P.EndedDown) state->sim_speed += 1;
	if(input->Keyboard.L.EndedDown) state->sim_speed -= 1;
	*/
	

//	===ADD SWAP===
	N64* swap;
	swap = s.current_state;
	s.current_state = s.next_state;
	s.next_state = swap;

}


