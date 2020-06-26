/* ========================================================================
   $File: bashe.cpp$
   $Date: 5/24/2020$
   $Revision: 1$
   $Creator: Alec Serra $
    ======================================================================== */

#include "base.h"
#include "matrix.cpp"

#define MAXIMUM_AGE 10
#define COLUMN_NUMBER 700.0f
#define ROW_NUMBER 700.0f





/*
Any live cell with two or three live neighbours survives.
Any dead cell with three live neighbours becomes a live cell.
All other live cells die in the next generation. Similarly, all other dead cells stay dead.
*/


int Update_State(Automata** a, int y_index, int x_index, Queue* new_state_queue)
{
	a[x_index][y_index].age++;
	
	if(a[x_index][y_index].age >= MAXIMUM_AGE)	
	{
		a[x_index][y_index].next_state = dead;
		a[x_index][y_index].age = 0;
		
	}
	if(x_index == 5 && y_index == 7)
	{
		printf("test \n");
	}
	int live_neighbors = 0;
	
	for(int x_location = x_index-1; x_location <= x_index+1; x_location++)
	{
		for(int y_location = y_index-1; y_location <= y_index+1; y_location++)
		{
			//clean this shit
			if(!(0 <= y_location && y_location <= COLUMN_NUMBER-1 &&
			0 <= x_location && x_location <= ROW_NUMBER-1))
			{
				break;
			}
			if(y_location == y_index && x_location == x_index)
			{
				y_location++;
			}
			if(a[x_location][y_location].current_state == alive)
			{
				live_neighbors++;
			}	
		}
	}
	
	if((live_neighbors == 2 || live_neighbors == 3) && a[x_index][y_index].current_state == alive)
	{
		a[x_index][y_index].next_state = alive;
	}
	else if(a[x_index][y_index].current_state == dead && live_neighbors == 3)
	{
		a[x_index][y_index].next_state = alive;
	}
	else
	{
		a[x_index][y_index].next_state = dead;
	}	
	
	if(a[x_index][y_index].current_state != a[x_index][y_index].next_state)
	{
		EnQueue(new_state_queue, &a[x_index][y_index]);
	}
	
	return (0);
}

int Init_Set(Set *s, R32 buffer_width, R32 buffer_height)
{
	s->scale = 1.0f;
	s->dimensions = {ROW_NUMBER, COLUMN_NUMBER};
	s->scale_factor = {buffer_width/s->dimensions.I, buffer_height/s->dimensions.J};
	s->a = (Automata**)malloc(sizeof(Automata*)*(int)s->dimensions.J);
	for(int y_index = 0; y_index < s->dimensions.J; y_index++)
	{
		s->a[y_index] = (Automata*)malloc(sizeof(Automata)*(int)s->dimensions.I);
	}
	
	for(int y_location = 0; y_location < s->dimensions.J; y_location++)
	{			
		for(int x_location = 0; x_location < s->dimensions.I; x_location++)
		{
			s->a[y_location][x_location].location.I = (R32)x_location;
			s->a[y_location][x_location].location.J = (R32)y_location;
			s->a[y_location][x_location].current_state = dead;
			s->a[y_location][x_location].next_state = dead;
			s->a[y_location][x_location].age = 0;
		}
	}
	//weird initial condition.
#if 1
	for(int y_index = 0; y_index < s->dimensions.J; y_index++)
	{
		for(int x_index = 0; x_index < s->dimensions.I; x_index++)
		{
			if(y_index%5 && x_index%3)
			{
				s->a[y_index][x_index-1].current_state = alive;
				s->a[y_index][x_index].current_state = alive;
				s->a[y_index][x_index+1].current_state = alive;
			}
		}
	}
#endif
#if 0
	for(int y_index = 2; y_index < s->dimensions.J; y_index++)
	{
		for(int x_index = 0; x_index < s->dimensions.I; x_index++)
		{
			if(y_index%4 && x_index%3)
			{
				s->a[y_index-1][x_index].current_state = alive;
				s->a[y_index][x_index].current_state = alive;
				s->a[y_index+1][x_index].current_state = alive;
			}
		}
	}
#endif
	return (0);
}
int Render_Backplane(Set *s, Vec_2 camera_position, RenderBuffer *buffer)
{
	ColorN32 c = {};
	c.Red = c.Green = c.Blue = 60;
	
	Vec_2 scale_factor_div_2 = s->scale_factor;
	mult_r32_vec_2(0.5f, scale_factor_div_2);
	
	Vec_2 point_1 = {0.0f, 0.0f};
	mult_vec_2_vec_2(point_1, s->scale_factor, point_1);
	sub_vec_2(point_1, scale_factor_div_2, point_1);
	add_vec_2(point_1, camera_position, point_1);
	mult_r32_vec_2(s->scale, point_1);
	
	
	Vec_2 point_2 = {COLUMN_NUMBER-1, ROW_NUMBER-1};						
	mult_vec_2_vec_2(point_2, s->scale_factor, point_2);
	add_vec_2(point_2, scale_factor_div_2, point_2);
	add_vec_2(point_2, camera_position, point_2);
	mult_r32_vec_2(s->scale, point_2);
	
	draw_rect_vec_2(point_1, point_2,
					buffer, &c, NULL);
	return (0);
}

int Render_Automata(Set *s, int y_index, int x_index, Vec_2 camera_position, RenderBuffer *buffer)
{
		ColorN32 c = {};
		
		
		if(s->a[x_index][y_index].current_state == dead)
		{
			c.Red = c.Green = c.Blue = 60;
		}
		else
		{
			c.Red = c.Green = c.Blue = (N8)(10*s->a[x_index][y_index].age) %(MAXIMUM_AGE+5);
		}
		Vec_2 point_1 = {};
		Vec_2 point_2 = {};
		Vec_2 scale_factor_div_2 = s->scale_factor;
		mult_r32_vec_2(0.5f, scale_factor_div_2);
	

		point_1 = s->a[x_index][y_index].location;
		mult_vec_2_vec_2(point_1, s->scale_factor, point_1);
		sub_vec_2(point_1, scale_factor_div_2, point_1);
		add_vec_2(point_1, camera_position, point_1);
		mult_r32_vec_2(s->scale, point_1);
		
		
		point_2 = s->a[x_index][y_index].location;						
		mult_vec_2_vec_2(point_2, s->scale_factor, point_2);
		add_vec_2(point_2, scale_factor_div_2, point_2);
		add_vec_2(point_2, camera_position, point_2);
		mult_r32_vec_2(s->scale, point_2);
		
		draw_rect_vec_2(point_1, point_2,
							buffer, &c, NULL);
		return (0);
}

void UpdateAndRender(Program_State *state, RenderBuffer *buffer, User_Input *input)
{
	local_persist Set s;
	local_persist Queue* new_state_queue;
	ColorN32 c = {};
	
	if(state->InitalRun)
	{
		state->InitalRun = false;
		
		state->counter = 0;
		state->sim_speed = 0;
		state->pause = 0;
		state->camera_position = {};
		//Init the system to be dead and setup location array for all elements.
		Init_Set(&s, (R32)buffer->Width, (R32)buffer->Height);
		new_state_queue = CreateQueue(buffer->Width*buffer->Height);
	}


	
	//Clear Screen	
	state->counter++;
	c.Red = 255;	
	draw_rect_z32(0, 0,
				buffer->Width, buffer->Height,
				buffer, &c);
	
	
		 

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
	
	Render_Backplane(&s, state->camera_position, buffer);
	for(int y_index = 0; y_index < s.dimensions.J; y_index++)
	{
		for(int x_index = 0; x_index < s.dimensions.I; x_index++)
		{	
			if(state->counter >= state->sim_speed)
			{
				Update_State(s.a, y_index, x_index, new_state_queue);
			}
			if(s.a[x_index][y_index].current_state == alive)	Render_Automata(&s, y_index, x_index, state->camera_position, buffer);
		}
	}
#if 1
	if(state->counter >= state->sim_speed)
	{
		while(new_state_queue->Size != 0)
		{
			Automata* a = DeQueue(new_state_queue);
			a->current_state = a->next_state;
		}
		
		state->counter = 0;
	}
#endif
#if 0
	if(state->counter >= state->sim_speed)
	{
		for(int y_index = 0; y_index < s.dimensions.J; y_index++)
		{
			for(int x_index = 0; x_index < s.dimensions.I; x_index++)
			{
				s.a[x_index][y_index].current_state = s.a[x_index][y_index].next_state;
			}
		}
		state->counter = 0;
	}
#endif
}



















Queue* CreateQueue(N32 Cap) 
{ 
    Queue* Q = (Queue*) malloc(sizeof(Queue)); 
    Q->Capacity = Cap; 
    Q->Front = Q->Size = 0;  
    Q->Rear = Cap - 1;  // This is important, see the enQueue 
    Q->Data = (Automata**) malloc(Q->Capacity * sizeof(Automata**)); 
    return Q;
} 

void EnQueue(Queue *Q, Automata *Data) 
{ 
    if (Q->Size == Q->Capacity)
	{		
        return; 
	}
	Q->Rear = (Q->Rear + 1)%Q->Capacity; 
	
	Q->Data[Q->Rear] = Data; 		
    Q->Size++; 
}

Automata* DeQueue(Queue* Q) 
{ 
	Automata *a = {};
    if (Q->Size == 0) 
        return a;
	if(Q->Data[Q->Front])
	{
		a = (Automata*)Q->Data[Q->Front]; 
    }
	Q->Front = (Q->Front + 1)%Q->Capacity; 
    Q->Size--; 
    return a; 
} 
  
Automata* GetFront(Queue* Q) 
{
	Automata *a = {};
    if (Q->Size == 0)
	{		
        return a; 
	}
	
	a = (Automata*)Q->Data[Q->Front]; 
	return a;
} 
  
Automata* GetRear(Queue* Q) 
{ 
    Automata *a = {};
    if (Q->Size == 0)
	{		
        return a; 
	}
	a = (Automata*)Q->Data[Q->Rear]; 
	return a;
} 



