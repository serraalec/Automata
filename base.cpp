/* ========================================================================
   $File: bashe.cpp$
   $Date: 5/24/2020$
   $Revision: 1$
   $Creator: Alec Serra $
    ======================================================================== */

#include "base.h"
#include "matrix.cpp"


global_variable char seed[512] = 
"MYbkhoascNB9KTiNhE5J6i6t0BqXdMPLdQE6QDdvuV1IAxEEagsuQGFl1ou7BeU"
"TceA6nHpFwRN6e5opQhp1MdTkdRLJtjExrLt5vbKxbeiMdUMsFeJDSfqiLITJCoFpt"
"N4kpBzMyFqrhkler4xkJZZQcf8PW8f9MdKGQd2T9woPegezBs0itEjZZg1IfHLVpS"
"G0AHmKnchWdS3kuhlIFRxPcf4tBoEjngg2jR3u5N9hTd79AycYhpWX7C5HKo5\0";


void Add_Node_Helper(struct Quad_Node* root, int desired_height) {
   if(root != NULL && root->height != desired_height) 
   {
		Vec_2 delta = root->position;
		delta.I = ((R32)root->height+1)/4.0f;
		delta.J =  root->height * (1.0f/desired_height);
		
		root->north_west = (Quad_Node*) malloc(sizeof(Quad_Node));
		root->north_west->height = root->height + 1;
		root->north_west->position = delta;
		delta.I += (((R32)root->height +1.0f)/2.0f);
		
		root->north_east = (Quad_Node*) malloc(sizeof(Quad_Node));
		root->north_east->height = root->height + 1;
		root->north_east->position = delta;
		delta.I += (((R32)root->height +1.0f)/2.0f);		
		
		root->south_west = (Quad_Node*) malloc(sizeof(Quad_Node));
		root->south_west->height = root->height + 1;
		root->south_west->position = delta;
		delta.I += (((R32)root->height +1.0f)/2.0f);
			
		root->south_east = (Quad_Node*) malloc(sizeof(Quad_Node));
		root->south_east->height = root->height + 1;
		root->south_east->position = delta;
		delta.I += (((R32)root->height +1.0f)/2.0f);
		
		Add_Node_Helper(root->north_west, desired_height);
		Add_Node_Helper(root->north_east, desired_height);
		Add_Node_Helper(root->south_west,  desired_height);
		Add_Node_Helper(root->south_east,  desired_height);
   }
}

//The initial height is how tall the tree is from the get go.
Quad_Tree* Init_Quad_Tree(int initial_height)
{
	Quad_Tree* tree;
	tree = (Quad_Tree*) malloc(sizeof(Quad_Tree));
	assert(tree != NULL);
	tree->height = initial_height;
	tree->root.height = 0;
	tree->root.current_state = miffed;
	tree->root.next_state = miffed;
	tree->root.position = {1.0f, 1.0f};
	Add_Node_Helper(&tree->root, initial_height);
	
	return tree;
}

/*
printGivenLevel(tree, level)
if tree is NULL then return;
if level is 1, then
    print(tree->data);
else if level greater than 1, then
    printGivenLevel(tree->left, level-1);
    printGivenLevel(tree->right, level-1);
*/

void Debug_View_Level(RenderBuffer *buffer, Quad_Node* node, int current_height, int tree_height)
{
	if(node == NULL || current_height < 0)
	{
		return;
	}
	if(current_height == 0)
	{
		Vec_2 screen_position = {node->position.I * buffer->Width/2.0f, node->position.J * buffer->Height/1.10f};
		ColorN32 c = {};
		Vec_2 screen_position_2 = screen_position;
		add_r32_vec_2(5.0f, screen_position_2);
		
		draw_rect_vec_2(screen_position, screen_position_2,	buffer, &c, NULL);
		
		
	}
#if 1
	else
	{
		Debug_View_Level(buffer, node->north_west, current_height-1, tree_height);
		Debug_View_Level(buffer, node->north_east, current_height-1, tree_height);
		Debug_View_Level(buffer, node->south_west, current_height-1, tree_height);
		Debug_View_Level(buffer, node->south_east, current_height-1, tree_height);
	}
#endif 
}
/*
printLevelorder(tree)
for d = 1 to height(tree)
   printGivenLevel(tree, d);
*/
void Debug_View_Tree(RenderBuffer *buffer, Quad_Tree* tree)
{
	for(int height_index = 0; height_index < tree->height; height_index++)
	{
		Debug_View_Level(buffer, &tree->root, height_index, tree->height);
	}
}

void UpdateAndRender(Program_State *state, RenderBuffer *buffer, User_Input *input)
{
	ColorN32 c = {};
	local_persist Quad_Tree* Tree;
	
	if(state->InitalRun)
	{
		state->InitalRun = false;
		
		state->counter = 0;
		state->sim_speed = 0;
		state->pause = 0;
		state->camera_position = {};
		Tree = Init_Quad_Tree(2);
		assert(Tree != NULL);
#if 0 
		Insert_Point(Tree, {0, 0}, alive);
		Insert_Point(Tree, {0, 10}, alive);
		Insert_Point(Tree, {10, 0}, alive);
		Insert_Point(Tree, {10, 10}, alive);
#endif

		c.Red = 255;	
		draw_rect_z32(0, 0,
				buffer->Width, buffer->Height,
				buffer, &c);


		Debug_View_Tree(buffer, Tree);
	}

	
	
	//Clear Screen	
	state->counter++;

	
	
		 

	Vec_2 delta = {};
	if(input->Keyboard.W.EndedDown) delta.J += 1.0f;
	if(input->Keyboard.D.EndedDown) 
		delta.I -= 1.0f;
	if(input->Keyboard.S.EndedDown) delta.J -= 1.0f;
	if(input->Keyboard.A.EndedDown) delta.I += 1.0f;
	
	if(input->Keyboard.C.EndedDown) state->pause = !state->pause;
	

	
	if(input->Keyboard.P.EndedDown) state->sim_speed += 1;
	if(input->Keyboard.L.EndedDown) state->sim_speed -= 1;
}


