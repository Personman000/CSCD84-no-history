/*
	CSC D84 - Unit 2 - MiniMax search and adversarial games

	This file contains stubs for implementing a MiniMax search
        procedure with alpha-beta pruning. Please read the assignment
	handout carefully - it describes the game, the data you will
	have to handle, and the search functions you must provide.

	Once you have read the handout carefully, implement your search
	code in the sections below marked with

	**************
	*** TO DO:
	**************

	Make sure to add it to your report.txt file - it will be marked!

	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Sep. 15
*/

#include "MiniMax_search.h"

const int UPP=0, RGT=1, DWN=2, LFT=3;	// Constants so I have less trouble dealing with directions->numbers

// List of coordinate modifications when traversing to a neighbor
int neighbor_values[4][2];

double MiniMax(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), int agentId, int depth, int maxDepth, double alpha, double beta)
{
 /*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in MiniMax_search_core_GL will call this function once per frame, and provide the following data
   
   Board and game layout:

	Exactly the same as for Assignment 1 - have a look at your code if you need a reminder of how the adjacency
	list and agent positions are stored.	

	Note that in this case, the path will contain a single move - at the top level, this function will provide
	an agent with the 'optimal' mini-max move given the game state.


   IMPORTANT NOTE: Mini-max is a recursive procedure. This function will need to fill-in the mini-max values for 
	 	   all game states down to the maximum search depth specified by the user. In order to do that,
		   the function needs to be called with the correct state at each specific node in the mini-max
		   search tree.

		   The game state is composed of:

			* Mouse, cat, and cheese positions (and number of cats and cheeses)
			
		   At the top level (when this function is called by the mini-max driver code), the game state
		   correspond to the current situation of the game. But once you start recursively calling
		   this function for lower levels of the search tree the positions of agents will have changed.
		   
		   Therefore, you will need to define local variables to keep the game state at each node of the
		   mini-max search tree, and you will need to update this state when calling recursively so that
		   the search does the right thing.

		   This function *must check* whether:
			* A candidate move results in a terminal configuration (cat eats mouse, mouse eats cheese)
			  at which point it calls the utility function to get a value
	 		* Maximum search depth has been reached (depth==maxDepth), at which point it will also call
			  the utility function to get a value
			* Otherwise, call recursively using the candidate configuration to find out what happens
			  deeper into the mini-max tree.

   Arguments:
		gr[graph_size][4]   		- This is an adjacency list for the maze
		path[1][2] 			- Your function will return the optimal mini-max move in this array.
		minmax_cost[size_X][size_Y]	- An array in which your code will store the
						  minimax value for maze locations expanded by
						  the search *when called for the mouse, not
						  for the cats!*

						  This array will be used to provide a visual 
						  display of minimax values during the game.

		cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
					 but there can be fewer). Only valid cat locations are 0 to (cats-1)
		cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
					     but possibly fewer). Valid locations are 0 to (cheeses-1)
		mouse_loc[1][2] - Mouse location - there can be only one!
		mode - Search mode selection:
					mode = 0 	- No alpha-beta pruning
					mode = 1	- Alpha-beta pruning

		(*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]);
				- This is a pointer to the utility function which returns a value for a specific game configuration

				   NOTE: Unlike the search assignment, this utility function also gets access to the graph so you can do any processing 					 that requires knowledge of the maze for computing the utility values.

				  * How to call the utility function from within this function : *
					- Like any other function:
						u = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
						
		agentId: Identifies which agent we are doing MiniMax for. agentId=0 for the mouse, agentId in [1, cats] for cats. Notice that recursive calls
                         to this function should increase the agentId to reflect the fact that the next level down corresponds to the next agent! For a game
                         with two cats and a mouse, the agentIds for the recursion should look like 0, 1, 2, 0, 1, 2, ...
	
		depth: Current search depth - whether this is a MIN or a MAX node depends both on depth and agentId.
		
		maxDepth: maximum desired search depth - once reached, your code should somehow return
			  a minimax utility value for this location.

		alpha. beta: alpha and beta values passed from the parent node to constrain search at this
			     level.

   Return values:
		Your search code will directly update data passed-in as arguments:
		
		- Mini-Max value	: Notice this function returns a double precision number. This is
					  the minimax value at this level of the tree. It will be used 
					  as the recursion backtracks filling-in the mini-max values back
					  from the leaves to the root of the search tree. 

		- path[1][2]		: Your MiniMax function will return the location for the agent's 
					  next location (i.e. the optimal move for the agent). 
		- minmax_cost[size_X][size_Y] 	:  Your search code will update this array to contain the
						   minimax value for locations that were expanded during
						   the search. This must be done *only* for the mouse.

						   Values in this array will be in the range returned by
						   your utility function.

		* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
	     	  or the location of the mouse - if you try, the driver code will know it *
			
		That's that, now, implement your solution!
 */

 /********************************************************************************************************
 * 
 * TO DO:	Implement code to perform a MiniMax search. This will involve a limited-depth BFS-like
 *              expansion. Once nodes below return values, your function will propagate minimax utilities
 *		as per the minimax algorithm.
 *	
 *		Note that if alpha-beta pruning is specified, you must keep track of alphas and betas
 *		along the path.
 *
 *		You can use helper functions if it seems reasonable. Add them to the MiniMax_search.h
 *		file and explain in your code why they are needed and how they are used.
 *
 *		Recursion should appear somewhere.
 *
 *		MiniMax cost: If the agentId=0 (Mouse), then once you have a MiniMax value for a location
 *		in the maze, you must update minmax_cost[][] for that location.
 *
 *		How you design your solution is up to you. But:
 *
 *		- Document your implementation by adding concise and clear comments in this file
 *		- Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/
//printf("min_urdl=[%f %f %f %f]\n",minmax_cost[12][22],minmax_cost[13][23],minmax_cost[12][24],minmax_cost[11][23]);


 // List of coordinate modifications when traversing to a neighbor
 // (x + neighbor_values[UPP][0], y + neighbor_values[UPP][1] = x, y of upwards neighbor)
 neighbor_values[UPP][0] = 0; neighbor_values[UPP][1] = -1;
 neighbor_values[RGT][0] = 1; neighbor_values[RGT][1] = 0;
 neighbor_values[DWN][0] = 0; neighbor_values[DWN][1] = 1;
 neighbor_values[LFT][0] = -1; neighbor_values[LFT][1] = 0;
 
 double max_util = -INFINITY;
 double max_util_index = 0;
 
 double min_util = INFINITY;
 double min_util_index = 0;
 
 // Update agents accordingly
 int next_agentId = agentId+1;
 if (next_agentId > cats)
 {
 	next_agentId = 0;
 }
 
 /*// If current_state is terminal_state OR If max depth has been reached
 int ind = mouse_loc[0][0] + (mouse_loc[0][1]*size_X);
 if(checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses)==1 || depth==maxDepth)
 {
	// Return utility(current_state)
	return utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
 }*/
 // If current_state is terminal_state OR If max depth has been reached
 int ind = mouse_loc[0][0] + (mouse_loc[0][1]*size_X);
 if(checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses)==1 || depth==maxDepth || (((int)(gr[ind][0]==0) + (int)(gr[ind][1]==0) + (int)(gr[ind][2]==0) + (int)(gr[ind][3]==0)) >= 3))
 {
	// Return utility(current_state)
	return utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
 }
 	
 // For each neighbor state around the current state
 int neighbor_index;
 double new_utility_value;
 for(neighbor_index = 0; neighbor_index < 4; neighbor_index++)
 {
 	// Only check connected nodes
 	if(gr[mouse_loc[0][0] + (mouse_loc[0][1]*size_X)][neighbor_index]==1)
 	{
	 	// If agent is mouse
	 	if(agentId==0)
	 	{
		 	// utilities[neighbor]=minmax_cost[neighbor]=Recurse with state:mouse moved to new node, agent=cat
		 	// move mouse to neighbor
		 	int new_mouse_loc[1][2];
		 	new_mouse_loc[0][0] = mouse_loc[0][0] + neighbor_values[neighbor_index][0];
		 	new_mouse_loc[0][1] = mouse_loc[0][1] + neighbor_values[neighbor_index][1];

			// check utility of game state with new mouse position
		 	new_utility_value = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, new_mouse_loc, mode, utility, next_agentId, depth+1, maxDepth, alpha, beta);

		 	// store utility into empty utility node, or smaller utility node
		 	if(minmax_cost[new_mouse_loc[0][0]][new_mouse_loc[0][1]]==0 || new_utility_value > minmax_cost[new_mouse_loc[0][0]][new_mouse_loc[0][1]])
		 	//if(true)
		 	{
		 		minmax_cost[new_mouse_loc[0][0]][new_mouse_loc[0][1]] = new_utility_value;
		 	}
		 	
		 	// get max utility
		 	if(new_utility_value > max_util)
		 	{
		 		max_util = new_utility_value;
		 		max_util_index = neighbor_index;
		 	}
		}
		// If agent is cat
		if(agentId>0)
		{
			// utilities[neighbor]=minmax_cost[neighbor]=Recurse with state:cat moved to new node, agent=next (if more cats, then next=next_cat. if no more cats, then next=mouse)
			// move cat to neighbor
			int new_cat_loc[10][2];
			memcpy(new_cat_loc, cat_loc, cats);
		 	new_cat_loc[agentId-1][0] = cat_loc[agentId-1][0] + neighbor_values[neighbor_index][0];
		 	new_cat_loc[agentId-1][1] = cat_loc[agentId-1][1] + neighbor_values[neighbor_index][1];

			// check utility of game state with new cat position
		 	new_utility_value = MiniMax(gr, path, minmax_cost, new_cat_loc, cats, cheese_loc, cheeses, mouse_loc, mode, utility, next_agentId, depth+1, maxDepth, alpha, beta);
		 	
		 	// store utility
		 	//minmax_cost[new_cat_loc[agentId-1][0]][new_cat_loc[agentId-1][1]] = new_utility_value;
		 	
		 	// get min utility
		 	if(new_utility_value < min_util)
		 	{
		 		min_util = new_utility_value;
		 		min_util_index = neighbor_index;
		 	}
		}
	}
 }
 //printf("nui=%d, agent=%d, urdl=[%f %f %f %f]\n",next_utility_index, agentId, minmax_cost[mouse_loc[0][0]][mouse_loc[0][1]-1],minmax_cost[mouse_loc[0][0]+1][mouse_loc[0][1]],minmax_cost[mouse_loc[0][0]][mouse_loc[0][1]+1],minmax_cost[mouse_loc[0][0]-1][mouse_loc[0][1]]);
 if(depth==0){
	 double next_utility=-INFINITY;
	 int next_utility_index;
	 double curr_neighbor_util;
printf("curr_n_util=[");
	 // Get best util out of neighbor utils
	 for(neighbor_index = 0; neighbor_index < 4; neighbor_index++)
	 {
		// Get largest util index
		curr_neighbor_util = minmax_cost[mouse_loc[0][0]+neighbor_values[neighbor_index][0]][mouse_loc[0][1]+neighbor_values[neighbor_index][1]];
		if(curr_neighbor_util>=next_utility && gr[mouse_loc[0][0] + (mouse_loc[0][1]*size_X)][neighbor_index]==1)
		{	
			// If two neighbors have equal utility, pick the one that's closer to the cheese
			if(curr_neighbor_util==next_utility)
			{
				int curr_neighbor_pos[1][2];
				curr_neighbor_pos[0][0] = mouse_loc[0][0]+neighbor_values[neighbor_index][0];
				curr_neighbor_pos[0][1] = mouse_loc[0][1]+neighbor_values[neighbor_index][1];
				double curr_neighbor_true_util = utility(cat_loc, cheese_loc, curr_neighbor_pos, cats, cheeses, depth, gr);
				
				int prev_neighbor_pos[1][2];
				prev_neighbor_pos[0][0] = mouse_loc[0][0]+neighbor_values[next_utility_index][0];
				prev_neighbor_pos[0][1] = mouse_loc[0][1]+neighbor_values[next_utility_index][1];
				double prev_neighbor_true_util = utility(cat_loc, cheese_loc, prev_neighbor_pos, cats, cheeses, depth, gr);
				
				if(curr_neighbor_true_util > prev_neighbor_true_util)
				{
					next_utility_index=neighbor_index;
				}
			}else
			{
				next_utility_index=neighbor_index;
			}
			
			next_utility=curr_neighbor_util;
		}
printf("%f ", curr_neighbor_util);
	 }
printf("] max=%f (next_index:%d)\n", next_utility, next_utility_index);
	 path[0][0] = mouse_loc[0][0] + neighbor_values[next_utility_index][0];//12;
	 path[0][1] = mouse_loc[0][1] + neighbor_values[next_utility_index][1];

//printf("MOUSELOC=[%d %d], PATH=[%d %d], DIRECTION=(%d %d)\n", mouse_loc[0][0], mouse_loc[0][1], path[0][0], path[0][1],neighbor_values[next_utility_index][0],neighbor_values[next_utility_index][1]);
//printf("nu=%f,nui=%d\n",next_utility, next_utility_index);
//printf("max=%f,%d | min=%f,%d\n", max_util, max_util_index, min_util, min_util_index);
//printf("urdl=[%f %f %f %f]\n",minmax_cost[mouse_loc[0][0]+neighbor_values[UPP][0]][mouse_loc[0][1]+neighbor_values[UPP][1]],minmax_cost[mouse_loc[0][0]+neighbor_values[RGT][0]][mouse_loc[0][1]+neighbor_values[RGT][1]],minmax_cost[mouse_loc[0][0]+neighbor_values[DWN][0]][mouse_loc[0][1]+neighbor_values[DWN][1]],minmax_cost[mouse_loc[0][0]+neighbor_values[LFT][0]][mouse_loc[0][1]+neighbor_values[LFT][1]]);
//printf("min_urdl=[%f %f %f %f]\n",minmax_cost[12][22],minmax_cost[13][23],minmax_cost[12][24],minmax_cost[11][23]);
//printf("\n");
 }
 
 // If mouse, return biggest util
 if(agentId==0)
 {
 	return max_util;
 }
 
 // If cat, return smallest util
 if(agentId>0)
 {
 	return min_util;
 }
}

double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4])
{
 /*
	This function computes and returns the utility value for a given game configuration.
	As discussed in lecture, this should return a positive value for configurations that are 'good'
	for the mouse, and a negative value for locations that are 'bad' for the mouse.

	How to define 'good' and 'bad' is up to you. Note that you can write a utility function
	that favours your mouse or favours the cats, but that would be a bad idea... (why?)

	Input arguments:

		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of cats
		cheeses - # of cheeses
		depth - current search depth
		gr - The graph's adjacency list for the maze

		These arguments are as described in A1. Do have a look at your solution!
 */

 // Get min distance from cats to mouse
 int i;
 double curr_cat_distance;
 double min_cat_distance = INFINITY;
 for(i=0;i<cats;i++)
 {
 	curr_cat_distance = pow(cat_loc[i][0] - mouse_loc[0][0], 2) + pow(cat_loc[i][1] - mouse_loc[0][1],2);
 	
 	if (curr_cat_distance < min_cat_distance)
 	{
 		min_cat_distance = curr_cat_distance;
 	}
 }
 
 // Get min distance from cheeses to mouse
 double curr_cheese_distance;
 double min_cheese_distance = INFINITY;
 for(i=0;i<cheeses;i++)
 {
 	curr_cheese_distance = pow(mouse_loc[0][0] - cheese_loc[i][0], 2) + pow(mouse_loc[0][1] - cheese_loc[i][1], 2);
 	
 	if (curr_cheese_distance < min_cheese_distance)
 	{
 		min_cheese_distance = curr_cheese_distance;
 	}
 }
 double max_distance = pow(size_X,2)+pow(size_Y,2);
 
 double min_distance = (max_distance-min_cheese_distance)/2;
 //double min_distance = (max_distance-min_cheese_distance)/2 - (max_distance-min_cat_distance)/20;
 
 if(min_cat_distance < 10)
 {
 	min_distance = min_cat_distance/4 - min_cheese_distance/2;
 }
 
 if(min_cheese_distance == 0)
 {
 	min_distance = max_distance;
 }
 
 if(min_cat_distance == 0)
 {
 	min_distance = -max_distance;
 }
 
 // Cut out dead ends
 int ind = mouse_loc[0][0] + (mouse_loc[0][1]*size_X);
 if((((int)(gr[ind][0]==0) + (int)(gr[ind][1]==0) + (int)(gr[ind][2]==0) + (int)(gr[ind][3]==0)) >= 3) && min_cheese_distance != 0)
 {
 	min_distance = -max_distance;
 }
 
//printf("md:%f\n",min_distance);
 // Return smallest distance
 //printf("MOUSE=[%d %d], CAT=[%d %d]\n",mouse_loc[0][0],mouse_loc[0][1],cat_loc[0][0],cat_loc[0][1]);
 //printf("mdb=%f,m-ch-d=%f\n",mouse_loc[0][0], mouse_loc[0][1],min_distance, min_cheese_distance);
 return min_distance;
}

int checkForTerminal(int mouse_loc[1][2],int cat_loc[10][2],int cheese_loc[10][2],int cats,int cheeses)
{
 /* 
   This function determines whether a given configuration constitutes a terminal node.
   Terminal nodes are those for which:
     - A cat eats the mouse
     or
     - The mouse eats a cheese
   
   If the node is a terminal, the function returns 1, else it returns 0
 */

 // Check for cats having lunch
 for (int i=0; i<cats; i++)
  if (mouse_loc[0][0]==cat_loc[i][0]&&mouse_loc[0][1]==cat_loc[i][1]) return(1);

 // Check for mouse having lunch
 for (int i=0; i<cheeses; i++)
  if (mouse_loc[0][0]==cheese_loc[i][0]&&mouse_loc[0][1]==cheese_loc[i][1]) return(1);

 return(0);

}

