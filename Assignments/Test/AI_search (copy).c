/*
	CSC D84 - Unit 1 - Search

	This file contains stubs for implementing the different search
	algorithms covered in the course. Please read the assignment
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

	Starter code: F.J.E., Jul. 15
	Updated: F.J.E., Jan. 18
*/

/**********************************************************************
% COMPLETE THIS TEXT BOX:
%
% 1) Student Name: Saad Ali
% 2) Student Name:		
%
% 1) Student number: 1002519269
% 2) Student number:
% 
% 1) UtorID: alisaad2
% 2) UtorID
% 
% We hereby certify that the work contained here is our own
%
% ______Saad Ali_______             _____________________
% (sign with your name)            (sign with your name)
***********************************************************************/

#include "AI_search.h"

bool find_node(int x, int y, int array[][2], int length)
{
	if(length > sizeof(array))
	{
		//printf("Given length is larger than array size. Array size: %d, Given Length: %d\n", sizeof(array), length);
		
		length = sizeof(array);
	}

	bool found = false;
	int i;
	for(i=0; i<length; i++)
	{
		found = found || (x == array[i][0] && y == array[i][1]);
	}
	
	return found;
}

bool find_open_node(int x, int y, int array[][3], int start, int length)
{
	if(start > length)
	{
		return false;
	}

	bool found = false;
	int i;
	for(i=start; i<=length; i++)
	{
		found = found || (x == array[i][0] && y == array[i][1]);
	}
	
	return found;
}

void search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]))
{
 /*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in AI_search_core_GL will call this function once per frame, and provide the following data
   
   Board and game layout:
	
	The game takes place on a grid of size 32x32, the file board_layout.h specifies the size and defines two
	constants 'size_X' and 'size_Y' for the horizontal and vertical size of the board, respectively. For our
	purposes, the grid of locations is represented by a graph with one node per grid location, so given
	the 32x32 cells, the graph has 1024 nodes. 

	To create a maze, we connect cell locations in the grid in such a way that a) there is a path from any
	grid location to any other grid location (i.e. there are no disconnected subsets of nodes in the graph),
	and b) there are loops.

	Since each node represents a grid location, each node can be connected to up to 4 neighbours in the
	top, right, bottom, and left directions respectively:

			node at (i,j-1)
				^
				|
	(node at i-1, j) <- node at (i,j) -> node at (i+1, j)
				|
				v
			node at (i,j+1)

	The graph is theredore stored as an adjacency list with size 1024 x 4, with one row per node in the
	graph, and 4 columns corresponding to the weight of an edge linking the node with each of its 4
	possible neighbours in the order towp, right, bottom, left (clockwise from top).

	Since all we care is whether nodes are connected. Weights will be either 0 or 1, if the weight is
	1, then the neighbouring nodes are connected, if the weight is 0, they are not. For example, if

	graph[i][0] = 0 
	graph[i][1] = 1
	graph[i][2] = 0
	graph[i][3] = 1

	then node i is connected to the right and left neighbours, but not to top or bottom.

	The index in the graph for the node corresponding to grid location (x,y) is

	index = x + (y*size_X) 		or in this case		index = x + (y*32)

	Conversely, if you have the index and want to figure out the grid location,

	x = index % size_X		or in this case		x = index % 32
	y = index / size_Y		or in this case		y = index / 32

	(all of the above are *integer* operations!)

	A path is a sequence of (x,y) grid locations. We store it using an array of size	
	1024 x 2 (since there are 1024 locations, this is the maximum length of any
	path that visits locations only once).

	Agent locations are coordinate pairs (x,y)	

   Arguments:
		gr[graph_size][4]   - This is an adjacency list for the maze
		path[graph_size][2] - An initially empty path for your code to fill.
				      In this case, empty means all entries are initially -1
		visit_order[size_X][size_Y]	- An array in which your code will store the
						  *order* in which grid locations were 
						  visited during search. For example, while
						  doing BFS, the initial location is the
						  start location, it's visit order is 1.
						  Then the search would expand the immediate
						  neighbours of the initial node in some order,
						  these would get a visit order of 2, 3, 4, and
						  5 respectively, and so on.
					
						  This array will be used to display the search
						  pattern generated by each search method.

		cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
					 but there can be fewer). Only valid cat locations are 0 to (cats-1)
		cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
					     but possibly fewer). Valid locations are 0 to (cheeses-1)
		mouse_loc[1][2] - Mouse location - there can be only one!
		mode - Search mode selection:
					mode = 0 	- BFS
					mode = 1	- DFS
					mode = 2	- A*

		(*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[10][2], int cats, int cheeses)
				- This is a pointer to one of the heuristic functions you will implement, either H_cost()
				  or H_cost_nokitty(). The driver in AI_search_core_GL will pass the appropriate pointer
				  depending on what search the user wants to run.
			
				  If the mode is 0 or 1, this pointer is NULL

				  * How to call the heuristic function from within this function : *
					- Like any other function:
						h = heuristic( x, y, cat_loc, cheese_loc, mouse_loc, cats, cheeses);

   Return values:
		Your search code will directly update data passed-in as arguments:

		- path[graph_size][2]	: Your search code will update this array to contain the path from
					  the mouse to one of the cheese chunks. The order matters, so
					  path[0][:] must be the mouse's current location, path[1][:]
					  is the next move for the mouse. Each successive row will contain
					  the next move toward the cheese, and the path ends at a location
					  whose coordinates correspond to one of the cheese chunks.
					  Any entries beyond that must remain set to -1
		- visit_order[size_X][size_Y] 	:  Your search code will update this array to contain the
						   order in which each location in the grid was expanded
						   during search. This means, when that particular location
						   was checked for being a goal, and if not a goal, had its
						   neighbours added as candidates for future expansion (in
						   whatever order is dictated by the search mode).

						   Note that since there are 1024 locations, the values in 
						   this array must always be in [0, 1023]. The driver code
						   will then display search order as a yellow-colored
						   brightness map where nodes expanded earlier will look
						   brighter.

		* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
	     	  or the location of the mouse - if you try, the driver code will know it *
			
		That's that, now, implement your solution!
 */

 /********************************************************************************************************
 * 
 * TO DO:	Implement code to carry out the different types of search depending on the
 *		mode.
 *
 *		You can do this by writing code within this single function (and being clever about it!)
 *		Or, you can use this function as a wrapper that calls a different search function
 *		 (BFS, DFS, A*) depending on the mode. Note that in the latter case, you will have
 *	    	  to inform your A* function somehow of what heuristic it's supposed to use.
 *
 *		Visiting Order: When adding the neighbours of a node to your list of candidates for
 *				expansion, do so in the order top, right, bottom, left.
 *
 *		NOTE: Your search functions should be smart enough to not choose a path that goes
 *		      through a cat! this is easily done without any heuristics.
 * 
 *		How you design your solution is up to you. But:
 *
 *		- Document your implementation by adding concise and clear comments in this file
 *		- Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/

 // Stub so that the code compiles/runs - The code below will be removed and replaced by your code!

 // Initialize neighbor list [][0]:node_x, [][1]:node_y, [][2]:node_parent_index (0=up,1=right,2=down,3=left)
 int open[graph_size][3];
 int open_index = 0;
 int open_start_index = 0;
 
 // Start neighbor list at mouse location
 open[0][0] = mouse_loc[0][0]; open[0][1] = mouse_loc[0][1];
 open[0][2] = -1;
 
 // Initialize visited index
 int visit_index = 1;
 
 // Initialize starting position
 int curr_x = mouse_loc[0][0], curr_y = mouse_loc[0][1];
 
 // Initialize other variables
 bool found = false;			// true when cheese is found
 int i;				// index integer
 int curr_graph_index;			// curr index using graph index (x + (y * size_X))
 int curr_node_index;			// temp variable for holding current index
 int curr_neighbor_x, curr_neighbor_y;	// Current neighbor coordinates
 
 const int UPP=0, RGT=1, DWN=2, LFT=3;	// Constants so I have less trouble dealing with directions->numbers
 
 bool in_open;				// true if current (neighbor) node is in OPEN list
 bool in_visit;			// true if current (neighbor) node is in VISITED list
 bool has_cat;				// true if current (neighbor) node has a cat on it

 // List of coordinate modifications when traversing to a neighbor
 // (x + neighbor_values[UPP][0], y + neighbor_values[UPP][1] = x, y of upwards neighbor)
 int neighbor_values[4][2];
 neighbor_values[UPP][0] = 0; neighbor_values[UPP][1] = -1;
 neighbor_values[RGT][0] = 1; neighbor_values[RGT][1] = 0;
 neighbor_values[DWN][0] = 0; neighbor_values[DWN][1] = 1;
 neighbor_values[LFT][0] = -1; neighbor_values[LFT][1] = 0;

 // BFS ---------------------------
 if (mode == 0)
 {
	 // Loop until cheese is found
	 while(found == false && open_start_index <= open_index)
	 {		
		// Get current node
		curr_x = open[open_start_index][0]; curr_y = open[open_start_index][1];
		curr_node_index = open_start_index;
		// Remove current node from Open list
		open_start_index++;

	 	// Check if current node has the cheese
		found = find_node(curr_x, curr_y, cheese_loc, cheeses);
		
		// If the node doesn't have cheese, then leggo fam
		if (found == false){
			// Run BFS Algorithm!
			
			// Update graph index
			curr_graph_index = curr_x + (curr_y * size_X);
		
			// Loop through all current node's neighbors
			for(i=0; i<4; i++){
			
				curr_neighbor_x = curr_x + neighbor_values[i][0];
				curr_neighbor_y = curr_y + neighbor_values[i][1];
				
				in_open = find_open_node(curr_neighbor_x, curr_neighbor_y, open, open_start_index, open_index);
				in_visit = visit_order[curr_neighbor_x][curr_neighbor_y] != 0;
				has_cat = find_node(curr_neighbor_x, curr_neighbor_y, cat_loc, cats);
				
				// If no wall, or not checked before, add neighbor to the OPEN list
				if(gr[curr_graph_index][i]==1 && in_open == false && in_visit == false && has_cat == false)
				{
					open_index++;
					open[open_index][0] = curr_neighbor_x; open[open_index][1] = curr_neighbor_y;
					open[open_index][2] = curr_node_index;	// Set parent as current index
				}
			}
			
			// Add current node to Visited list
			visit_order[curr_x][curr_y] = visit_index;
			visit_index++;
		}
	 }
	 
	 // BFS-PATH-BUILD ---------------------------------------
	 int temp_path[graph_size][2];			// temp path array to be flipped for actual path array
	 int path_index = 0;				// index to build path
	 int search_index = open_start_index-1;	// search index for finding parent nodes
	 
	 while(search_index != 0)
	 {		
	 	// Place current nodes into path
	  	temp_path[path_index][0] = curr_x;
	 	temp_path[path_index][1] = curr_y;

	 	path_index++;

		// Update search index with parent index
		search_index = open[search_index][2];
		
	 	// Update current nodes
		curr_x = open[search_index][0];
	 	curr_y = open[search_index][1];
	 }
	 // Add dangling node
	 temp_path[path_index][0] = curr_x;
	 temp_path[path_index][1] = curr_y;
	 
	 // Invert path
	 int j = 0;
	 for(i=path_index; i>=0; i--)
	 {
	 	path[j][0] = temp_path[i][0];
	 	path[j][1] = temp_path[i][1];
	 	j++;
	 }
 }
 // DFS ---------------------------
 else if(mode == 1)
 {
 }
 
 return;

}

int H_cost(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
 /*
	This function computes and returns the heuristic cost for location x,y.
	As discussed in lecture, this means estimating the cost of getting from x,y to the goal. 
	The goal is cheese. Which cheese is up to you.
	Whatever you code here, your heuristic must be admissible.

	Input arguments:

		x,y - Location for which this function will compute a heuristic search cost
		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of cats
		cheeses - # of cheeses
		gr - The graph's adjacency list for the maze

		These arguments are as described in the search() function above
 */

 return(1);		// <-- Evidently you will need to update this.
}

int H_cost_nokitty(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
 /*
	This function computes and returns the heuristic cost for location x,y.
	As discussed in lecture, this means estimating the cost of getting from x,y to the goal. 
	The goal is cheese. 

	However - this time you want your heuristic function to help the mouse avoid being eaten.
	Therefore: You have to somehow incorporate knowledge of the cats' locations into your
	heuristic cost estimate. How well you do this will determine how well your mouse behaves
	and how good it is at escaping kitties.

	This heuristic *does not have to* be admissible.

	Input arguments have the same meaning as in the H_cost() function above.
 */

 return(1);		// <-- Evidently you will need to update this.
}

