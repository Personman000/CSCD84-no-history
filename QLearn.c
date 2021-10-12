/*
	CSC D84 - Unit 3 - Reinforcement Learning
	
	This file contains stubs for implementing the Q-Learning method
	for reinforcement learning as discussed in lecture. You have to
	complete two versions of Q-Learning.
	
	* Standard Q-Learning, based on a full-state representation and
	  a large Q-Table
	* Feature based Q-Learning to handle problems too big to allow
	  for a full-state representation
	    
	Read the assignment handout carefully, then implement the
	required functions below. Sections where you have to add code
	are marked

	**************
	*** TO DO:
	**************

	If you add any helper functions, make sure you document them
	properly and indicate in the report.txt file what you added.
	
	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Jan. 16
*/

#include "QLearn.h"
// Helpers Start-------------------------

// Returns the maximum allowed action of the QTable at state s
double get_max_allowed_qsa(double *QTable, int s, double gr[max_graph_size][4], int graph_index)
{

  int i;
  int action_index;
  double max_qsa = -INFINITY;
  double curr_qsa;
  for (i=0; i<4; i++)
  {
  	curr_qsa = *(QTable+(4*s)+i);
  	
  	if (curr_qsa > max_qsa && gr[graph_index][i] == 1)
  	{
  		max_qsa = curr_qsa;
  		action_index = i;
  	}
  }
  
  return action_index;
}

// Returns the maximum value of the QTable at state s
double get_max_qsa(double *QTable, int s)
{

  int i;
  int action_index;
  double max_qsa = -INFINITY;
  double curr_qsa;
  for (i=0; i<4; i++)
  {
  	curr_qsa = *(QTable+(4*s)+i);
  	
  	if (curr_qsa > max_qsa)
  	{
  		max_qsa = curr_qsa;
  	}
  }
  
  return max_qsa;
}

// Returns the current encoded state
int get_state(int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
	return (mouse_pos[0][0]+(mouse_pos[0][1]*size_X)) + ((cats[0][0]+(cats[0][1]*size_X))*graph_size) + ((cheeses[0][0]+(cheeses[0][0]*size_X))*graph_size*graph_size);
}

// Helpers End---------------------------

void QLearn_update(int s, int a, double r, int s_new, double *QTable)
{
 /*
   This function implementes the Q-Learning update as stated in Lecture. It 
   receives as input a <s,a,r,s'> tuple, and updates the Q-table accordingly.
   
   Your work here is to calculate the required update for the Q-table entry
   for state s, and apply it to the Q-table
     
   The update involves two constants, alpha and lambda, which are defined in QLearn.h - you should not 
   have to change their values. Use them as they are.
     
   Details on how states are used for indexing into the QTable are shown
   below, in the comments for QLearn_action. Be sure to read those as well!
 */
 
  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  
  // Get Q(s,a)
  double qsa = *(QTable+(4*s)+a);
  
  // Get max(a')Q(s',a')
  double max_qsa_new = get_max_qsa(QTable, s_new);
  
  
  *(QTable+(4*s)+a) += (alpha) * ((r + (lambda * max_qsa_new)) - qsa);
}

int QLearn_action(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, double *QTable, int size_X, int graph_size)
{
  /*
     This function decides the action the mouse will take. It receives as inputs
     - The graph - so you can check for walls! The mouse must never move through walls
     - The mouse position
     - The cat position
     - The chees position
     - A 'pct' value in [0,1] indicating the amount of time the mouse uses the QTable to decide its action,
       for example, if pct=.25, then 25% of the time the mouse uses the QTable to choose its action,
       the remaining 75% of the time it chooses randomly among the available actions.
       
     Remember that the training process involves random exploration initially, but as training
     proceeds we use the QTable more and more, in order to improve our QTable values around promising
     actions.
     
     The value of pct is controlled by QLearn_core_GL, and increases with each round of training.
     
     This function *must return* an action index in [0,3] where
        0 - move up
        1 - move right
        2 - move down
        3 - move left
        
     QLearn_core_GL will print a warning if your action makes the mouse cross a wall, or if it makes
     the mouse leave the map - this should not happen. If you see a warning, fix the code in this
     function!
     
   The Q-table has been pre-allocated and initialized to 0. The Q-table has
   a size of
   
        graph_size^3 x 4
        
   This is because the table requires one entry for each possible state, and
   the state is comprised of the position of the mouse, cat, and cheese. 
   Since each of these agents can be in one of graph_size positions, all
   possible combinations yield graph_size^3 states.
   
   Now, for each state, the mouse has up to 4 possible moves (up, right,
   down, and left). We ignore here the fact that some moves are not possible
   from some states (due to walls) - it is up to the QLearn_action() function
   to make sure the mouse never crosses a wall. 
   
   So all in all, you have a big table.
        
   For example, on an 8x8 maze, the Q-table will have a size of
   
       64^3 x 4  entries
       
       with 
       
       size_X = 8		<--- size of one side of the maze
       graph_size = 64		<--- Total number of nodes in the graph
       
   Indexing within the Q-table works as follows:
   
     say the mouse is at   i,j
         the cat is at     k,l
         the cheese is at  m,n
         
     state = (i+(j*size_X)) + ((k+(l*size_X))*graph_size) + ((m+(n*size_X))*graph_size*graph_size)
     ** Make sure you undestand the state encoding above!
     
     Entries in the Q-table for this state are

     *(QTable+(4*state)+a)      <-- here a is the action in [0,3]
     
     (yes, it's a linear array, no shorcuts with brackets!)
     
     NOTE: There is only one cat and once cheese, so you only need to use cats[0][:] and cheeses[0][:]
   */
  
  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/  

  double rand_val = ((double)(rand() % 101))/100;
  int s = get_state(mouse_pos, cats, cheeses, size_X, graph_size);
  int action;
  
  int mouse_index = mouse_pos[0][0] + (size_X * mouse_pos[0][1]);
  
  // Return random action or Q_action
  if (rand_val >= pct)
  {
  	action = rand() % 4;
  	while(gr[mouse_index][action] != 1)
  	{
  		action = rand() % 4;
	}
  }
  else
  {
  	action = get_max_allowed_qsa(QTable, s, gr, mouse_index);
  }
  
  return action;
}

double QLearn_reward(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
    This function computes and returns a reward for the state represented by the input mouse, cat, and
    cheese position. 
    
    You can make this function as simple or as complex as you like. But it should return positive values
    for states that are favorable to the mouse, and negative values for states that are bad for the 
    mouse.
    
    I am providing you with the graph, in case you want to do some processing on the maze in order to
    decide the reward. 
        
    This function should return a maximim/minimum reward when the mouse eats/gets eaten respectively.      
   */

   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/ 
   
   double reward = 0;
   
   if(mouse_pos[0][0]==cats[0][0] && mouse_pos[0][1]==cats[0][1])
   {
   	reward = -graph_size;
   }
   else if(mouse_pos[0][0]==cheeses[0][0] && mouse_pos[0][1]==cheeses[0][1])
   {
   	reward = graph_size;
   }

   return reward;
}

void feat_QLearn_update(double gr[max_graph_size][4],double weights[25], double reward, int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
    This function performs the Q-learning adjustment to all the weights associated with your
    features. Unlike standard Q-learning, you don't receive a <s,a,r,s'> tuple, instead,
    you receive the current state (mouse, cats, and cheese potisions), and the reward 
    associated with this action (this is called immediately after the mouse makes a move,
    so implicit in this is the mouse having selected some action)
    
    Your code must then evaluate the update and apply it to the weights in the weight array.    
   */
  
   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/        
   // curr_features, new_features = double array[num_of_features]
   //
   // evaluate_features(curr_features, ??prev_mouse_pos??)
   // evaluate_features(new_features, mouse_pos)
   //
   // Qs = weights[0]*curr_features[0] + weights[1]*curr_features[1] +...
   // Qs'= weights[0]*new_features[0]  + weights[1]*new_features[1]  +...
   //
   // weights[i] += alpha*(reward + lambda*(Qs' - Qs))*curr_features[i]

   double next_maxU;
   int next_maxA;
   maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &next_maxU, &next_maxA);
   
   int new_mouse_pos[1][2];
   switch(next_maxA)
   {
	case 0: new_mouse_pos[0][0] = mouse_pos[0][0]; new_mouse_pos[0][1] = mouse_pos[0][1] - 1; break;
	case 1: new_mouse_pos[0][0] = mouse_pos[0][0] + 1; new_mouse_pos[0][1] = mouse_pos[0][1]; break;
	case 2: new_mouse_pos[0][0] = mouse_pos[0][0]; new_mouse_pos[0][1] = mouse_pos[0][1] + 1; break;
	case 3: new_mouse_pos[0][0] = mouse_pos[0][0] - 1; new_mouse_pos[0][1] = mouse_pos[0][1]; break;
   }

   double curr_features[25];
   double new_features[25];
   
   evaluateFeatures(gr, curr_features, mouse_pos, cats, cheeses, size_X, graph_size);
   evaluateFeatures(gr, new_features, new_mouse_pos, cats, cheeses, size_X, graph_size);
   
   double Qs = Qsa(weights, curr_features);
   double Qs_prime = Qsa(weights, new_features);
   
   int i;
   for(i=0; i<numFeatures; i++)
   {
   	weights[i] += alpha*(reward + (lambda*Qs_prime - Qs))*curr_features[i];
   }
}	

int feat_QLearn_action(double gr[max_graph_size][4],double weights[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, int size_X, int graph_size)
{
  /*
    Similar to its counterpart for standard Q-learning, this function returns the index of the next
    action to be taken by the mouse.
    
    Once more, the 'pct' value controls the percent of time that the function chooses an optimal
    action given the current policy.
    
    E.g. if 'pct' is .15, then 15% of the time the function uses the current weights and chooses
    the optimal action. The remaining 85% of the time, a random action is chosen.
    
    As before, the mouse must never select an action that causes it to walk through walls or leave
    the maze.    
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/        
  double rand_val = ((double)(rand() % 101))/100;
  int s = get_state(mouse_pos, cats, cheeses, size_X, graph_size);
  int action;
  
  int mouse_index = mouse_pos[0][0] + (size_X * mouse_pos[0][1]);
  
  // Return random action or Q_action
  if (rand_val >= pct)
  {
  	action = rand() % 4;
  	while(gr[mouse_index][action] != 1)
  	{
  		action = rand() % 4;
	}
  }
  // Return maxQsa
  else
  {
  	double maxU;
  	int maxA;
  	maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &maxU, &maxA);
  	
  	action = maxA;
  }

  return action;
}

void evaluateFeatures(double gr[max_graph_size][4],double features[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
   This function evaluates all the features you defined for the game configuration given by the input
   mouse, cats, and cheese positions. You are free to define up to 25 features. This function will
   evaluate each, and return all the feature values in the features[] array.
   
   Take some time to think about what features would be useful to have, the better your features, the
   smarter your mouse!
   
   Note that instead of passing down the number of cats and the number of cheese chunks (too many parms!)
   the arrays themselves will tell you what are valid cat/cheese locations.
   
   You can have up to 5 cats and up to 5 cheese chunks, and array entries for the remaining cats/cheese
   will have a value of -1 - check this when evaluating your features!
  */

   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/      
   int i;
   
   // features[0] = distance to closest cheese
   double curr_cheese_dist;
   double min_cheese_dist = INFINITY;
   i = 0;
   while(cheeses[i][0] != -1 && cheeses[i][1] != -1)
   {
   	curr_cheese_dist = pow(cheeses[i][0] - mouse_pos[0][0], 2) + pow(cheeses[i][1] - mouse_pos[0][1], 2);

   	if(curr_cheese_dist < min_cheese_dist)
   	{
   		min_cheese_dist = curr_cheese_dist;
   	}
   	i++;
   }
   
   // features[1] = distance to closest cat
   double curr_cat_dist;
   double min_cat_dist = INFINITY;
   i = 0;
   while(cats[i][0] != -1 && cats[i][1] != -1)
   {
   	curr_cat_dist = pow(cats[i][0] - mouse_pos[0][0], 2) + pow(cats[i][1] - mouse_pos[0][1], 2);

   	if(curr_cat_dist < min_cat_dist)
   	{
   		min_cat_dist = curr_cat_dist;
   	}
   	i++;
   }
   
   // Set features
   features[0] = min_cheese_dist/graph_size;
   features[1] = min_cat_dist/graph_size;
}

double Qsa(double weights[25], double features[25])
{
  /*
    Compute and return the Qsa value given the input features and current weights
   */

  /***********************************************************************************************
  * TO DO: Complete this function
  ***********************************************************************************************/  
  
  int i;
  double dot_prod = 0;
  for(i=0; i<numFeatures; i++)
  {
  	dot_prod += weights[i]*features[i];
  }
  
  return dot_prod;
}

void maxQsa(double gr[max_graph_size][4],double weights[25],int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size, double *maxU, int *maxA)
{
 /*
   Given the state represented by the input positions for mouse, cats, and cheese, this function evaluates
   the Q-value at all possible neighbour states and returns the max. The maximum value is returned in maxU
   and the index of the action corresponding to this value is returned in maxA.
   
   You should make sure the function does not evaluate moves that would make the mouse walk through a
   wall. 
  */
 
   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/  
   
  *maxU = -INFINITY;
  *maxA = -1;
  
  int mouse_index = mouse_pos[0][0] + (mouse_pos[0][1] * size_X);
  
  int new_mouse_pos[1][2];
  
  double curr_qsa;
  double curr_features[25];
  
  int action;
  for(action=0; action<4; action++)
  {
  	// Only check for valid actions
  	if(gr[mouse_index][action] == 1)
  	{
  		// Get new mouse position
	  	switch(action)
		{
			case 0: new_mouse_pos[0][0] = mouse_pos[0][0]; new_mouse_pos[0][1] = mouse_pos[0][1] - 1; break;
			case 1: new_mouse_pos[0][0] = mouse_pos[0][0] + 1; new_mouse_pos[0][1] = mouse_pos[0][1]; break;
			case 2: new_mouse_pos[0][0] = mouse_pos[0][0]; new_mouse_pos[0][1] = mouse_pos[0][1] + 1; break;
			case 3: new_mouse_pos[0][0] = mouse_pos[0][0] - 1; new_mouse_pos[0][1] = mouse_pos[0][1]; break;
		}
		
		// Evaluate features given current action was taken
		evaluateFeatures(gr, curr_features, new_mouse_pos, cats, cheeses, size_X, graph_size);
		
		// Evalate Qsa given current action was taken
		curr_qsa = Qsa(weights, curr_features);
		
		// If the current action's Qsa is greater, update it
		if(curr_qsa > *maxU)
		{
			*maxU = curr_qsa;
			*maxA = action;
		}
	}

  }
}

/***************************************************************************************************
 *  Add any functions needed to compute your features below 
 *                 ---->  THIS BOX <-----
 * *************************************************************************************************/
 
