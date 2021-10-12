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
 // List of coordinate modifications when traversing to a neighbor
 // (x + neighbor_values[UPP][0], y + neighbor_values[UPP][1] = x, y of upwards neighbor)
 neighbor_values[UPP][0] = 0; neighbor_values[UPP][1] = -1;
 neighbor_values[RGT][0] = 1; neighbor_values[RGT][1] = 0;
 neighbor_values[DWN][0] = 0; neighbor_values[DWN][1] = 1;
 neighbor_values[LFT][0] = -1; neighbor_values[LFT][1] = 0;
 
 double max_util = -graph_size;
 int max_util_index = 0;
 
 double min_util = graph_size;
 int min_util_index = 0;
 
 // Update agents accordingly
 int next_agentId = agentId+1;
 if (next_agentId > cats)
 {
 	next_agentId = 0;
 }

 // If current_state is terminal_state OR If max depth has been reached OR dead end has been found
 int ind = mouse_loc[0][0] + (mouse_loc[0][1]*size_X);
 if(checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses)==1 || depth==maxDepth || (((int)(gr[ind][0]==0) + (int)(gr[ind][1]==0) + (int)(gr[ind][2]==0) + (int)(gr[ind][3]==0)) >= 3))
 {
	// Return utility(current_state)
	int u = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
	minmax_cost[mouse_loc[0][0]][mouse_loc[0][1]] = u;
	return u;
 }
 
 // Loop through each neighbor -------------------------------------------------------------------
 int neighbor_index;
 double new_utility_value;
 for(neighbor_index = 0; neighbor_index < 4; neighbor_index++)
 {
 	// If agent is mouse ----------------------------------------------------------------------
 	if(agentId==0)
 	{
 		 	// Only check connected nodes
	 	if(gr[mouse_loc[0][0] + (mouse_loc[0][1]*size_X)][neighbor_index]==1)
	 	{
		 	// utilities[neighbor]=minmax_cost[neighbor]=Recurse with state:mouse moved to new node, agent=cat
		 	// move mouse to neighbor
		 	int new_mouse_loc[1][2];
		 	new_mouse_loc[0][0] = mouse_loc[0][0] + neighbor_values[neighbor_index][0];
		 	new_mouse_loc[0][1] = mouse_loc[0][1] + neighbor_values[neighbor_index][1];

			// check utility of game state with new mouse position
			// skip if alpha-beta-pruning is on, and alpha >= beta
			if(((mode==1) & (alpha < beta)) || mode==0)
			{
		 		new_utility_value = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, new_mouse_loc, mode, utility, next_agentId, depth+1, maxDepth, alpha, beta);
		 	}

			// update alpha
			if(new_utility_value > alpha)
			{
				alpha = new_utility_value;
			}
		 	
		 	// get max utility
		 	if(new_utility_value > max_util)
		 	{
		 		max_util = new_utility_value;
		 		max_util_index = neighbor_index;
		 	}
	 	}
	}
	// If agent is cat ------------------------------------------------------------------------
	if(agentId>0)
	{
		if(gr[cat_loc[agentId-1][0] + (cat_loc[agentId-1][1]*size_X)][neighbor_index]==1)
	 	{
			// move cat to neighbor
			int new_cat_loc[10][2];
			
			int i;
			for(i=0;i<cats;i++)
			{
				new_cat_loc[i][0] = cat_loc[i][0];
				new_cat_loc[i][1] = cat_loc[i][1];
			}
			
		 	new_cat_loc[agentId-1][0] = cat_loc[agentId-1][0] + neighbor_values[neighbor_index][0];
		 	new_cat_loc[agentId-1][1] = cat_loc[agentId-1][1] + neighbor_values[neighbor_index][1];

			// check utility of game state with new cat position
		 	if(((mode==1) & (alpha < beta)) || mode==0)
			{
		 		new_utility_value = MiniMax(gr, path, minmax_cost, new_cat_loc, cats, cheese_loc, cheeses, mouse_loc, mode, utility, next_agentId, depth+1, maxDepth, alpha, beta);
		 	}
		 	
		 	// update beta
			if(new_utility_value < alpha)
			{
				beta = new_utility_value;
			}
		 	
		 	// get min utility
		 	if(new_utility_value < min_util)
		 	{
		 		min_util = new_utility_value;
		 		min_util_index = neighbor_index;
		 	}
		 }
	}
 }
 
 // Final check ------------------------------------------------------------------------------------------------------------------------------------------
 if(depth==0){
	 double next_utility=-INFINITY;
	 int next_utility_index;
	 double curr_neighbor_util;

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
	 }
	 path[0][0] = mouse_loc[0][0] + neighbor_values[next_utility_index][0];//12;
	 path[0][1] = mouse_loc[0][1] + neighbor_values[next_utility_index][1];
 }
 
 
 // Recursive return -------------------------------------------------------------------------------------------------------------------------
 // If mouse, return biggest util
 if(agentId==0)
 {
 	minmax_cost[mouse_loc[0][0]][mouse_loc[0][1]] = max_util;
 	return max_util;
 }
 
 // If cat, return smallest util
 if(agentId>0)
 {
 	minmax_cost[mouse_loc[0][0]][mouse_loc[0][1]] = min_util;
 	return min_util;
 }
}


// Utility -------------------------------------------------------------------------------------------------------------------------------
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
 // return bad value if dead end (unless the dead end is a terminal node)
 int ind = mouse_loc[0][0] + (mouse_loc[0][1]*size_X);
 if((((int)(gr[ind][0]!=1) + (int)(gr[ind][1]!=1) + (int)(gr[ind][2]!=1) + (int)(gr[ind][3]!=1)) >= 3) && checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses)!=1)
 {
 	return -graph_size;
 }
 
 // Get min distance from cats to mouse
 int i;
 double curr_cat_distance;
 double min_cat_distance = INFINITY;
 int min_cat_distance_index = 0;
 for(i=0;i<cats;i++)
 {
 	curr_cat_distance = pow(cat_loc[i][0] - mouse_loc[0][0], 2) + pow(cat_loc[i][1] - mouse_loc[0][1],2);
 	
 	if (curr_cat_distance < min_cat_distance)
 	{
 		min_cat_distance = curr_cat_distance;
 		min_cat_distance_index = i;
 	}
 }
 
 // Get min distance from cheeses to mouse
 double curr_cheese_distance;
 double min_cheese_distance = INFINITY;
 int min_cheese_distance_index = 0;
 for(i=0;i<cheeses;i++)
 {
 	curr_cheese_distance = pow(mouse_loc[0][0] - cheese_loc[i][0], 2) + pow(mouse_loc[0][1] - cheese_loc[i][1], 2);
 	
 	if (curr_cheese_distance < min_cheese_distance)
 	{
 		min_cheese_distance = curr_cheese_distance;
 		min_cheese_distance_index = i;
 	}
 }

 int cat_distance = pathfind(mouse_loc, cat_loc[min_cat_distance_index][0], cat_loc[min_cat_distance_index][1], gr);
 int cheese_distance = pathfind(mouse_loc, cheese_loc[min_cheese_distance_index][0], cheese_loc[min_cheese_distance_index][1], gr);
 int count;
 if(cat_distance < cheese_distance){count = (cat_distance - cheese_distance);}
 else{count = -cheese_distance;}
 
 return count+10;
}


// Pathfind  -------------------------------------------------------------------------------------------------------------------------------
int pathfind(int mouse_loc[1][2], int target_x, int target_y, double gr[graph_size][4])
{
 // Initialize node lists
 int visited[size_X][size_Y] = {{0,0}};
 
 int open_end_index = 0;
 int open_start_index = 0;
 
 int open[graph_size][2];
 int open_parents[graph_size];
 open[open_start_index][0] = mouse_loc[0][0]; open[open_start_index][1] = mouse_loc[0][1];
 open_parents[open_start_index] = -1;
 
 bool found = false;
 
 int curr_x;
 int curr_y;
 
 int curr_node_index;
 int curr_graph_index;
 
 int curr_neighbor_x;
 int curr_neighbor_y;
 
 int i;
 
 bool in_open;
 bool in_visit;
 bool has_cat;

 // Loop until cheese is found
 while(found == false && open_start_index <= open_end_index)
 {		
	// Get current node
	curr_x = open[open_start_index][0];
	curr_y = open[open_start_index][1];
	curr_node_index = open_start_index;
	
	// Remove current node from Open list
	open_start_index++;

 	// Check if current node has the cheese
	found = (curr_x == target_x && curr_y == target_y);
	
	// If the node doesn't have cheese, then leggo fam
	if (found == false){
		// Run BFS Algorithm!
		
		// Update graph index
		curr_graph_index = curr_x + (curr_y * size_X);
	
		// Loop through all current node's neighbors
		for(i=0; i<4; i++){
			
			// If neighbor is connected, check it
			if(gr[curr_graph_index][i]==1)
			{
			
				curr_neighbor_x = curr_x + neighbor_values[i][0];
				curr_neighbor_y = curr_y + neighbor_values[i][1];
				
				if(curr_neighbor_x != mouse_loc[0][0] || curr_neighbor_y != mouse_loc[0][1])
				{
				
					in_open = find_node(curr_neighbor_x, curr_neighbor_y, open, open_start_index, open_end_index+1);
					in_visit = visited[curr_neighbor_x][curr_neighbor_y] == 1;
					
					// If no wall, or not checked before, add neighbor to the OPEN list
					if(in_open == false && in_visit == false)
					{
						open_end_index++;
						open[open_end_index][0] = curr_neighbor_x; open[open_end_index][1] = curr_neighbor_y;
						open_parents[open_end_index] = curr_node_index;	// Set parent as current index
					}
				}
			}
		}
		
		// Add current node to Visited list
		visited[curr_x][curr_y] = 1;
	}
 }
 int count = 0;
 int curr_index = open_start_index-1;
 while(open_parents[curr_index] != 0)
 {
 	curr_index = open_parents[curr_index];
 	count = count+1;
 }
 if(open[0][0] == target_x && open[0][1] == target_y)
 {
 	return -graph_size;
 }
 else if(found)
 {
	if(count == 0){return -1;}
	else{return count;}
 }
 else
 {
 	return graph_size;
 }
}

// Helpers ---------------------------------------------------------------------------------------------
bool find_node(int x, int y, int array[][2], int start, int end)
{
	bool found = false;
	int i;
	for(i=start; i<end; i++)
	{
		found = found || (x == array[i][0] && y == array[i][1]);
	}
	
	return found;
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

