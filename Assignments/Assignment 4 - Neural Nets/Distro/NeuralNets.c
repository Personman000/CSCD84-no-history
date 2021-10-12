/*
	CSC D84 - Unit 4 - Neural Networks
	
	This file contains stubs for implementing your Neural Nets classifier.
	
	You will implement a single layer network, and 2-layer network,
	and you will study what is the effect of having different numbers
	of neurons in the network, as well as what happens when you used
	different sigmoid functions.
	    
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

	Starter code: F.J.E. Feb. 16
*/

#include "NeuralNets.h"


int train_1layer_net(double sample[INPUTS],int label,double (*sigmoid)(double input), double weights_io[INPUTS][OUTPUTS])
{
 /*
  *   This is your main training function for 1-layer networks. Recall from lecture that we have a simple,
  *  direct connection between inputs and output neurons (the only layer present here). What we are doing
  *  in effect is training 10 different classifiers, each of which will learn to distinguish one of our
  *  training digits.
  * 
  *  Inputs:
  *   sample  -  Array with the pixel values for the input digit - in this case a 28x28 image (784 pixels)
  *              with values in [0-255], plus one bias term (last entry in the array) which is always 1
  *   label  -   Correct label for this digit (our target class)
  *   sigmoid -  The sigmoid function being used, which will be either the logistic function or the hyperbolic
  *              tangent. You have to implement the logistic function, but math.h provides tanh() already
  *   weights_io - Array of weights connecting inputs to output neurons, weights[i][j] is the weight from input
  *                i to output neuron j. This array has a size of 785x10.
  *
  *   Return values:
  *     An int in [0,9] corresponding to the class that your current network has chosen for this training
  *   sample.
  * 
  */

  /**********************************************************************************************************
  *   TO DO: Implement this function! it must compute for a given input digit the network's output values
  *          for each output neuron, and cause the weights to be updated based on the error for each neuron
  *          and according to the backpropagation algorithm discussed in lecture.
  *          
  *          You will need to complete feedforward_1layer(), backprop_1layer(), and logistic() in order to
  *          be able to complete this function.
  ***********************************************************************************************************/

  // Initialize output array
  double activations[OUTPUTS] = {0};

  // Calculate all outputs
  feedforward_1layer(sample, sigmoid, weights_io, activations);

  // Update weights
  backprop_1layer(sample, activations, sigmoid, label, weights_io);

  // Return predicted value
  return classify_1layer(sample, label, sigmoid, weights_io);
}

int classify_1layer(double sample[INPUTS],int label,double (*sigmoid)(double input), double weights_io[INPUTS][OUTPUTS])
{
 /*
  *   This function classifies an input sample given the current network weights. It returns a class in
  *  [0,9] corresponding to the digit the network has decided is present in the input sample
  * 
  *  Inputs:
  *   sample  -  Array with the pixel values for the input digit - in this case a 28x28 image (784 pixels)
  *              with values in [0-255], plus one bias term (last entry in the array) which is always 1
  *   label  -   Correct label for this digit (our target class)
  *   sigmoid -  The sigmoid function being used, which will be either the logistic function or the hyperbolic
  *              tangent. You have to implement the logistic function, but math.h provides tanh() already
  *   weights_io - Array of weights connecting inputs to output neurons, weights[i][j] is the weight from input
  *                i to output neuron j. This array has a size of 785x10.
  *
  *   Return values:
  *     An int in [0,9] corresponding to the class that your current network has chosen for this training
  *   sample.
  * 
  */

  /**********************************************************************************************************
  *   TO DO: Implement this function! 
  *          
  *          You will need to complete feedforward_1layer(), and logistic() in order to
  *          be able to complete this function.
  ***********************************************************************************************************/
  
  // Calculate outputs
  double activations[OUTPUTS] = {0};
  feedforward_1layer(sample, sigmoid, weights_io, activations);
  
  // Return largest output neuron index
  double curr_output;
  double max_output = -INFINITY;
  int max_output_index = 0;
  
  int output_i;
   for (output_i=0; output_i<OUTPUTS; output_i++)
   {
   	curr_output = activations[output_i];
	if (curr_output > max_output)
	{
		max_output = curr_output;
		max_output_index = output_i;
	}
   }
   
   return max_output_index;
}

void feedforward_1layer(double sample[785], double (*sigmoid)(double input), double weights_io[INPUTS][OUTPUTS], double activations[OUTPUTS])
{
 /*
  *  This function performs the feedforward pass of the network's computation - it propagates information
  *  from input to output, determines the input to each neuron, and calls the sigmoid function to
  *  calculate neuron activation.
  * 
  *  Inputs:
  *    sample -      The input sample (see above for a description)
  *    sigmoid -     The sigmoid function being used
  *    weights_op -  Array of current network weights
  *    activations - Array where your function will store the resulting activation for each output neuron
  * 
  *  Return values:
  *    Your function must update the 'activations' array with the output value for each neuron
  * 
  *  NOTE - You must *scale* the input to the sigmoid function using the SIGMOID_SCALE value. Otherwise
  *         the neurons will be totally saturated and learning won't happen.
  */ 
 
  /*******************************************************************************************************
   * TO DO: Complete this function. You will need to ib NeuralNets.c:285mplement logistic() in order for this to work
   *        with a logistic activation function.
   ******************************************************************************************************/

   // Calculate outputs using inputs and send the values to activations array
   int output_i;
   int input_i;
   for (output_i=0; output_i<OUTPUTS; output_i++)
   {
	for (input_i=0; input_i<INPUTS; input_i++)
	{
		// Calculate sum of weights*inputs
		activations[output_i] += weights_io[input_i][output_i] * sample[input_i];
	}
	// Calculate sigmoid of sum(weights*inputs)
	activations[output_i] = sigmoid(SIGMOID_SCALE * activations[output_i]);
   }
}

void backprop_1layer(double sample[INPUTS], double activations[OUTPUTS], double (*sigmoid)(double input), int label, double weights_io[INPUTS][OUTPUTS])
{
  /*
   *  This function performs the core of the learning process for 1-layer networks. It takes
   *  as input the feed-forward activation for each neuron, the expected label for this training
   *  sample, and the weights array. Then it updates the weights in the array so as to minimize
   *  error across neuron outputs.
   * 
   *  Inputs:
   * 	sample - 	Input sample (see above for details)
   *    activations - 	Neuron outputs as computed above
   *    sigmoid -	Sigmoid function in use
   *    label - 	Correct class for this sample
   *    weights_io -	Network weights
   * 
   *  You have to:
   * 		* Determine the target value for each neuron
   * 			- This depends on the type of sigmoid being used, you should think about
   * 			  this: What should the neuron's output be if the neuron corresponds to
   * 			  the correct label, and what should the output be for every other neuron?
   * 		* Compute an error value given the neuron's target
   * 		* Compute the weight adjustment for each weight (the learning rate is in NeuralNets.h)
   */
  
   /***************************************************************************************************
    * TO DO: Implement this function to compute and apply the weight updates for all weights in
    *        the network. You will need to find a way to figure out which sigmoid function you're
    *        using. Then use the procedure discussed in lecture to compute weight updates.
    * ************************************************************************************************/
   
   // Calculate all output errors derivs and update weights accordingly
   
   char sigmoid_name;
   if (sigmoid(0) == 0.5)
   {
   	sigmoid_name = 'l';
   }
   else if (sigmoid(0) == 0)
   {
   	sigmoid_name = 'h';
   }
   else
   {
   	printf("Unrecognized sigmoid function\n");
   }
   
   double input;
   double f_a_i;
   double target;
   double error_deriv;
   
   int output_i;
   int input_i;
   for (output_i=0; output_i<OUTPUTS; output_i++)
   {
	for (input_i=0; input_i<INPUTS; input_i++)
	{
		input = sample[input_i];
		f_a_i = activations[output_i];						// f(A(I))
		
		// Calculate error deriv
		if (sigmoid_name == 'l')
		{
			target = label==output_i ? 1 : 0;	// Target is maximal if active neuron matches label. Minimal ow
			error_deriv = input * f_a_i * (1 - f_a_i) * (target - f_a_i);	// I_a * f(A(I)) * (1 - f(A(I))) * (T_b - O_b)
		}
		else if (sigmoid_name == 'h')
		{
			target = label==output_i ? 1 : -1;	// Target is maximal if active neuron matches label. Minimal ow
			error_deriv = input * (1 - pow(f_a_i, 2)) * (target - f_a_i);	// I_a * (1 - tanh^2(A(I))) * (T_b - O_b)
		}
		
		// Update weights
		weights_io[input_i][output_i] += ALPHA * error_deriv;
	}
   }
}

int train_2layer_net(double sample[INPUTS],int label,double (*sigmoid)(double input), int units, double weights_ih[INPUTS][MAX_HIDDEN], double weights_ho[MAX_HIDDEN][OUTPUTS])
{
 /*
  *   This is your main training function for 2-layer networks. Now you have to worry about the hidden
  *  layer at this time. *Do not work on this until you have completed the 1-layer network*.
  * 
  *  Inputs:
  *   sample  -  Array with the pixel values for the input digit - in this case a 28x28 image (784 pixels)
  *              with values in [0-255], plus one bias term (last entry in the array) which is always 1
  *   label  -   Correct label for this digit (our target class)
  *   sigmoid -  The sigmoid function being used, which will be either the logistic function or the hyperbolic
  *              tangent. You have to implement the logistic function, but math.h provides tanh() already
  *   units   -  Number of units in the hidden layer
  *   weights_ih - Array of weights connecting inputs to hidden-layer neurons, weights_ih[i][j] is the 
  *                weight from input i to hidden neuron j. This array has a size of units 785 x 10.
  *   weights_ho - Array of weights connecting hidden-layer units to output neurons, weights_ho[i][j] is the 
  *                weight from hidden unit i to output neuron j. This array has a size of units x 10.
  *
  *   Return values:
  *     An int in [0,9] corresponding to the class that your current network has chosen for this training
  *   sample.
  * 
  */

  /**********************************************************************************************************
  *   TO DO: Implement this function! it must compute for a given input digit the activation for each
  * 	     neuron in the hidden layer, and then use that to compute output neuron activations.
  * 
  *          Then it must use these activations along with the input sample's label to update the weights
  * 	     across the whole network
  *          
  *          You will need to complete feedforward_2layer(), backprop_2layer(), and logistic() in order to
  *          be able to complete this function.
  ***********************************************************************************************************/
  // Initialize output array
  double activations_ih[units] = {0};
  double activations_ho[OUTPUTS] = {0};

  // Calculate all outputs
  feedforward_2layer(sample, sigmoid, weights_ih, weights_ho, activations_ih, activations_ho, units);

  // Update weights
  backprop_2layer(sample, activations_ih, activations_ho, sigmoid, label, weights_ih, weights_ho, units);
  
//int i;int h;int o;
//for(i=0;i<5;i++){for(h=0;h<units;h++){printf("Weight I->H (%d->%d): %f\n", i, h, weights_ih[i][h]);}}
//for(h=0;h<units;h++){for(o=0;o<OUTPUTS;o++){printf("Weight H->O (%d->%d): %f\n", h, o, weights_ho[h][o]);}}
//i=1;h=2;printf("Weight I->H (%d->%d): %f\t", i, h, weights_ih[1][2]);printf("Activation H (%d): %f\n", h, activations_ih[h]);
//h=2;o=3;printf("Weight H->O (%d->%d): %f\t", i, h, weights_ho[1][2]);printf("Activation O (%d): %f\n", o, activations_ho[o]);

  // Return predicted value
  return classify_2layer(sample, label, sigmoid, units, weights_ih, weights_ho);
}

int classify_2layer(double sample[INPUTS],int label,double (*sigmoid)(double input), int units, double weights_ih[INPUTS][MAX_HIDDEN], double weights_ho[MAX_HIDDEN][OUTPUTS])
{
 /*
  *   This function takes an input sample and classifies it using the current network weights. It returns
  *  an int in [0,9] corresponding to which digit the network thinks is present in the input sample.
  * 
  *  Inputs:
  *   sample  -  Array with the pixel values for the input digit - in this case a 28x28 image (784 pixels)
  *              with values in [0-255], plus one bias term (last entry in the array) which is always 1
  *   label  -   Correct label for this digit (our target class)
  *   sigmoid -  The sigmoid function being used, which will be either the logistic function or the hyperbolic
  *              tangent. You have to implement the logistic function, but math.h provides tanh() already
  *   units   -  Number of units in the hidden layer
  *   weights_ih - Array of weights connecting inputs to hidden-layer neurons, weights_ih[i][j] is the 
  *                weight from input i to hidden neuron j. This array has a size of units 785 x 10.
  *   weights_ho - Array of weights connecting hidden-layer units to output neurons, weights_ho[i][j] is the 
  *                weight from hidden unit i to output neuron j. This array has a size of units x 10.
  *
  *   Return values:
  *     An int in [0,9] corresponding to the class that your current network has chosen for this training
  *   sample.
  * 
  */

  /**********************************************************************************************************
  *   TO DO: Implement this function! 
  *          
  *          You will need to complete feedforward_2layer(), and logistic() in order to
  *          be able to complete this function.
  ***********************************************************************************************************/
  // Calculate outputs
  double activations_ih[units] = {0};
  double activations_ho[OUTPUTS] = {0};
  feedforward_2layer(sample, sigmoid, weights_ih, weights_ho, activations_ih, activations_ho, units);
  
  // Return largest output neuron index
  double max_output = -INFINITY;
  int max_output_index = 0;
  
  int ho_index;
  double ho_output; 	
  for (ho_index=0; ho_index<OUTPUTS; ho_index++)
  {
	ho_output = activations_ho[ho_index];
	if(ho_output > max_output)
	{
		max_output = ho_output;
		max_output_index = ho_index;
	}
  }
   
  return max_output_index;
}


void feedforward_2layer(double sample[INPUTS], double (*sigmoid)(double input), double weights_ih[INPUTS][MAX_HIDDEN], double weights_ho[MAX_HIDDEN][OUTPUTS], double h_activations[MAX_HIDDEN],double activations[OUTPUTS], int units)
{
 /*
  *  Here, implement the feedforward part of the two-layer network's computation.
  * 
  *  Inputs:
  *    sample -      The input sample (see above for a description)
  *    sigmoid -     The sigmoid function being used
  *    weights_ih -  Array of current input-to-hidden weights
  *    weights_ho -  Array of current hidden-to-output weights
  *    h_activations - Array of hidden layer unit activations
  *    activations   - Array of activations for output neurons
  *    units -         Number of units in the hidden layer
  * 
  *  Return values:
  *    Your function must update the 'activations' and 'h_activations' arrays with the output values for each neuron
  * 
  *  NOTE - You must *scale* the input to the sigmoid function using the SIGMOID_SCALE value. Otherwise
  *         the neurons will be totally saturated and learning won't happen.
  */ 
 
  /*******************************************************************************************************
   * TO DO: Complete this function. You will need to implement logistic() in order for this to work
   *        with a logistic activation function.
   ******************************************************************************************************/

  /**************************************************************************************************
   * Important note - scaling inputs to neurouns is critical to ensure the neurons don't saturate.
   *                  Scaling for the hidden layer works just like it did for the 1 layer net,
   * 		       simply scale your input by SIGMOID_SCALE. However, for the output layer,
   *                  the scaling factor has to be adjusted by the factor
   *                  SIGMOID_SCALE*(MAX_HIDDEN/units).
   **************************************************************************************************/
   // Activations for Input -> Hidden
   int h_index;
   int ih_index;
   for (h_index=0; h_index<units; h_index++)
   {
	for (ih_index=0; ih_index<INPUTS; ih_index++)
	{
		// Given input, calculate output and plug into activations array
		h_activations[h_index] += weights_ih[ih_index][h_index] * sample[ih_index];
	}
	h_activations[h_index] = sigmoid(SIGMOID_SCALE * h_activations[h_index]);
   }
   
   // Activations for Hidden -> Output
   int o_index;
   int ho_index;
   for (o_index=0; o_index<OUTPUTS; o_index++)
   {
	for (ho_index=0; ho_index<units; ho_index++)
	{
		// Given input, calculate output and plug into activations array
		activations[o_index] += weights_ho[ho_index][o_index] * h_activations[ho_index];
	}
	activations[o_index] = sigmoid((SIGMOID_SCALE * (MAX_HIDDEN/units)) * activations[o_index]);
   }
}

void backprop_2layer(double sample[INPUTS],double h_activations[MAX_HIDDEN], double activations[OUTPUTS], double (*sigmoid)(double input), int label, double weights_ih[INPUTS][MAX_HIDDEN], double weights_ho[MAX_HIDDEN][OUTPUTS], int units)
{
  /*
   *  This function performs the core of the learning process for 2-layer networks. It performs
   *  the weights update as discussed in lecture. Note that you require the current weights
   *  between the hidden and output layers in order to update the weights from input to hidden,
   *  however the backprop. algorithm would have you update that weight first. So mind the order
   *  of updates and keep track of what you need.
   * 
   *  Inputs:
   * 	sample - 	Input sample (see above for details)
   *    h_activations - Hidden-layer activations
   *    activations -   Output-layer activations
   *    sigmoid -	Sigmoid function in use
   *    label - 	Correct class for this sample
   *    weights_ih -	Network weights from inputs to hidden layer
   *    weights_ho -    Network weights from hidden layer to output layer
   *    units -         Number of units in the hidden layer
   * 
   *  You have to:
   * 		* Determine the target value for each neuron
   * 			- This depends on the type of sigmoid being used, you should think about
   * 			  this: What should the neuron's output be if the neuron corresponds to
   * 			  the correct label, and what should the output be for every other neuron?
   * 		* Compute an error value given the neuron's target
   * 		* Compute the weight adjustment for each weight (the learning rate is in NeuralNets.h)
   */
  
   /***************************************************************************************************
    * TO DO: Implement this function to compute and apply the weight updates for all weights in
    *        the network. You will need to find a way to figure out which sigmoid function you're
    *        using. Then use the procedure discussed in lecture to compute weight updates.
    * ************************************************************************************************/
   // Calculate all output errors derivs and update weights accordingly
   
   // Recognize which sigmoid function is being used
   char sigmoid_name;
   if (sigmoid(0) == 0.5)
   {
   	sigmoid_name = 'l';
   }
   else if (sigmoid(0) == 0)
   {
   	sigmoid_name = 'h';
   }
   else
   {
   	printf("Unrecognized sigmoid function\n");
   }

   int h_index;
   int i_index;
   int o_index;
   
   double input;
   double target;
   
   // Calculate errors for Hidden -> Output ->
   double weight_updates_ho[units][OUTPUTS];
   
   double f_a_i;
   double error_deriv;
   
   for (o_index=0; o_index<OUTPUTS; o_index++)
   {
	for (h_index=0; h_index<units; h_index++)
	{
		input = h_activations[h_index];
		f_a_i = activations[o_index];						// f(A(I))
		
		// Calculate error deriv
		if (sigmoid_name == 'l')
		{
			target = label==o_index ? 1 : 0;	// Target is maximal if active neuron matches label. Minimal ow
			error_deriv = input * f_a_i * (1 - f_a_i) * (target - f_a_i);	// I_a * f(A(I)) * (1 - f(A(I))) * (T_b - O_b)
		}
		else if (sigmoid_name == 'h')
		{
			target = label==o_index ? 1 : -1;	// Target is maximal if active neuron matches label. Minimal ow
			error_deriv = input * (1 - pow(f_a_i, 2)) * (target - f_a_i);	// I_a * (1 - tanh^2(A(I))) * (T_b - O_b)
		}
		
		// Store weight updates
		weight_updates_ho[h_index][o_index] = ALPHA * error_deriv;
	}
   }

   // Calculate errors for Input -> Hidden ->
   double weight_updates_ih[INPUTS][units];
   
   double deriv_AI_wab;
   double deriv_Ob_AI;
   double deriv_Errb_Ob;
   
   double target_j;
   double output_j;

   // dErr_b/dO_b
   double deriv_fx_AjI[OUTPUTS];
   for (o_index=0; o_index<OUTPUTS; o_index++)
   {
	if (sigmoid_name == 'l')
	{
		deriv_fx_AjI[o_index] = activations[o_index] * (1 - activations[o_index]);	// f(A(I)) * (1 - f(A(I)))
	}
	else if (sigmoid_name == 'h')
	{
		deriv_fx_AjI[o_index] = (1 - pow(activations[o_index], 2));			// (1 - tanh^2(A(I)))
	}
   }
   
   // w_ab = w_ab + a * dErr_b/dw_ab
   for (h_index=0; h_index<units; h_index++)
   {
   	for (i_index=0; i_index<INPUTS; i_index++)
   	{
   		// dA(I)/dw_ab
		deriv_AI_wab = sample[i_index];
				
		// dO_b/dA(I)
		if (sigmoid_name == 'l')
		{
			deriv_Ob_AI = h_activations[h_index] * (1 - h_activations[h_index]);	// f(A(I)) * (1 - f(A(I)))
		}
		else if (sigmoid_name == 'h')
		{
			deriv_Ob_AI = (1 - pow(h_activations[h_index], 2));			// (1 - tanh^2(A(I)))
		}
		
		// dErr_b/dO_b
		deriv_Errb_Ob = 0;
		for (o_index=0; o_index<OUTPUTS; o_index++)
		{
			if (sigmoid_name == 'l')
			{
				target_j = label==o_index ? 1 : 0;	// Target is maximal if active neuron matches label. Minimal ow
			}
			else if (sigmoid_name == 'h')
			{
				target_j = label==o_index ? 1 : -1;	// Target is maximal if active neuron matches label. Minimal ow
			}
			
			output_j = activations[o_index];
			
			deriv_Errb_Ob += weights_ho[h_index][o_index] * deriv_fx_AjI[o_index] * (target_j - output_j);	//sum(w_bj * df(X)/dA_j(I) * (T_j - O_j))
		}
		
		// Store weight updates
		weight_updates_ih[i_index][h_index] = ALPHA * deriv_AI_wab * deriv_Ob_AI * deriv_Errb_Ob;
	}
   }
   
   // Update weights for Hidden -> Output
   for (o_index=0; o_index<OUTPUTS; o_index++)
   {
	for (h_index=0; h_index<units; h_index++)
	{
		weights_ho[h_index][o_index] += weight_updates_ho[h_index][o_index];
	}
   }
   
   // Update weights for Input -> Hidden
   for (h_index=0; h_index<units; h_index++)
   {
   	for (i_index=0; i_index<INPUTS; i_index++)
   	{
		weights_ih[i_index][h_index] += weight_updates_ih[i_index][h_index];
	}
   }
}

double logistic(double input)
{
 // This function returns the value of the logistic function evaluated on input
 // TO DO: Implement this function!
 return 1/(1+exp(-input));
}
