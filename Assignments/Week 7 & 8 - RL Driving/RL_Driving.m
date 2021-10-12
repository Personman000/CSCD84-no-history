%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CSC D84 - Winter 2020 - UTSC
%
% In this script, we will use reinforcement
% learning to train a little car to drive
% on a curvy highway without hitting junk
% left on the road.
%
% Specifically, you will implement the
% value iteration method to find a policy
% that helps the car avoid crashing either
% by going off road, or by hitting junk
% that may appear randomly on the road.
%
% Starter code: F. Estrada, Feb. 20, 2020
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% This function should compute and return
% the Policy, which is simply the index to
% the action the car must make for each 
% state in order to maximize its reward.
%
% Rewards are as follows:
% * Not crashing (for every unit of motion forward)  Rg
% * Leaving the road                                 Ro
% * Crashing against junk                            Rj
% 
% Rg, Ro, and Rj are input parameters, so we
% can see if anything happens when we change their
% relative weight.
% 
% A state corresponds to a single horizontal
% row of the image of the road - the current
% state corresponds to the row in which the
% car is sitting.
%
% The road is 10 blocks wide, each block
% has to be one of: Vegetation, Road, Car,
% or Junk.
%
% Car can overlap anything, Junk only appears
% on the road.
%
% The road itself is 4 blocks wide, and can 
% begin anywhere between positions 1 and 5
% in the image row. Road locations are always
% contiguous.
%
% Examples: (V=vegetation, R=road, C=car, J=junk)
%
% 0 1 2 3 4 5 6 7 8 9   <-- Block index
% -------------------
% V R R C R V V V V V   (road is in blocks 2-5)
% V V R C R R V V V V   (road is in blocks 3-6)
% V V R J C R V V V V   (road is in blocks 3-6)
% V V V V R R R R V V   (road is in blocks 5-8)
%
% State indexes:
%
% The state consists of two rows in the driving
% sim. The current row, which contains the car,
% and the next row - so the algorithm can figure
% out what's coming next. With this in mind, the
% components of the State index are
%
% a) Position of the road in the current row
%    Gives us the first value i1
%    i1=0   when road starts at block 1 in the row
%    i1=1   when road starts at block 2 in the row
%    .
%    .
%    i1=4   when road starts at block 5 in the row
%
%    (notice these are ALL the valid locations for
%    the road)
%
% b) Position of the car in current row
%    Gives us a value i2 such that:
%    i2=0 if car is in block 0
%    i2=1 if car is in block 1
%    .
%    .
%    i2=9 if car is in block 9
%
%    (notice the car can be anywhere, even outside the road)
%
% c) Position of junk (junk only can be on the road) in current row
%    Gives us a value i3 such that:
%    i3=0 if there is NO junk on the road
%    i3=1 if the junk is at the leftmost block of the road
%    i3=2 if the junk is the second block from the left
%    i3=3 if the junk is the third block from the left
%    i3=4 if the junk is the rightmost block of the road
%
%    (notice these are all the valid locations for junk)
%
% d) Position of the road in the NEXT row 
%    i4=0   when road starts at block 1 in the row
%    i4=1   when road starts at block 2 in the row
%    .
%    .
%    i4=4   when road starts at block 5 in the row
%
% e) Position of junk (junk only can be on the road) in NEXT row
%    Gives us a value i3 such that:
%    i5=0 if there is NO junk on the road
%    i5=1 if the junk is at the leftmost block of the road
%    i5=2 if the junk is the second block from the left
%    i5=3 if the junk is the third block from the left
%    i5=4 if the junk is the rightmost block of the road
%
% From i1 - i5 we can find the state index for any
% valid configuration:
%
% I= i1 + (i2*5) + (i3*50) + (i4*250) + (i5*1250)
%
% There are 50 valid combiations of i1 and i2, so with 5
% values for i3, we get 250 combinations, then 5 possible
% values for i4 makes 1250 combinations, and finally 5 more
% for i5 makes a total of 6250 states.
%
% NOTE: States start at 0, but indexing in Matlab starts
%       at 1, which is annoying in this case, but there you
%       have it - you'll need to use state+1 when indexing
%       into the Q_table, V_star, or Policy.
%
%  *** NOT all state indexes correspond to valid 
%      configurations! e.g. the road position can
%      only change by +/- 1 block from row to row,
%      the car's position can only change by +/-
%      one block between consecutive states, etc.
%
% From a state index s we can work out the positions
% of things as follows:
%
% i5 = floor(s/1250);
% i4 = floor((s-(i5*1250))/250);
% i3 = floor((s-(i5*1250)-(i4*250))/50);
% i2 = floor((s-(i5*1250)-(i4*250)-(i3*50))/5);
% i1 = s-(i5*1250)-(i4*250)-(i3*50)-(i2*5);
%
% I have added a helpful function that returns the i1,
% i2, and i3 from a given index I, and one that turns
% a stats s into I1 and I2.
%
% Actions:
%
% At each step, the car has 3 possible actions:
% 1 - Move one block left
% 2 - Stay on the current block
% 3 - Move one block right
%
% So your Q table for this exercise will have a size of
% 6250*3 entries, and this is the table you have to 
% fill with values in order to compute the policy.
%
% The function below takes in the number of rounds of
% value iteration we wish to do, and the reward values,
% and returns the Policy after the specified number of
% rounds.
%
% function [Policy]=RL_Driving(N, gamma, Rg, Ro, Rj)
%   IN:
%    N - number of rounds of value iteration
%    gamma - the discount factor for future rewards
%    Rg, Ro, Rj - reward values as described above
%   OUT:
%    Policy - a 6520x1 array with the optimal action
%             for each state (the value for non-valid
%             states doesn't matter)
%    Q_table - The Q table after value iteration
%    V_star  - The value of each state after value iteration
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [Policy, Q_table, V_star]=RL_Driving(N, gamma, Rg, Ro, Rj)

DEBUG=0;
Policy=ones(6250,1)*2;            %  Initially the car will stick to its position.
Q_table=ones(6250,3)*-Inf;        %  Your Q table, 6250 states, 3 actions
V_star=zeros(6250);           %  Optimal value for states
                              %  In class we said we could initialize
                              %  this to the reward value for terminal
                              %  states - but here because the car
                              %  can accumulate rewards by not crashing
                              %  (and these states are NOT terminal)
                              %  we will just initialize to zero and
                              %  let the value iteration process 
                              %  figure things out.
                              
for round=1:N                 % For the expected number of training rounds
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %% TO DO:
    %%
    %% Implement here the value iteration method to update the Q table
    %% and the optimal value for each state according to the update
    %% equations from class. 
    %%
    %% You also have to complete the function R(s,a) below as per the
    %% specifications above for the rewards in this game
    %%
    %% You have the following problem - you need to figure out what are
    %% the possible next states you need to consider in your update.
    %% For this, consider that the current state s includes the current
    %% row and the next - call the next row Nx.
    %% The successor states s' *current* row will be Nx with the car
    %% at the position resulting from the chosen action a.
    %% Successor states s' have a *next* row such that the road
    %% can appear in the same location as their current row, or
    %% one block to the left, or one block to the right. 
    %% Additionally, junk can appear anywhere on the road, or 
    %% not be there at all.
    %% You have to include all possibilities in your update computation!
    %%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % While you're solving this, spend some time pondering the
    % fact the car doesn't even have to drive a single time 
    % down the road for you to find the policy! we're brute
    % forcing every possible state so we don't need to run any
    % simulations here!
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    for state=0:6249
        for action=1:3

            [i1,i2,i3,i4,i5]=indexesFromState(state);
            
            car_next_pos = (i2-(action-2));
            
            % Only loop through viable states
            if(i1 ~= -1 & i2 ~= -1 & i3 ~= -1 & i4 ~= -1 & i5 ~= -1 & car_next_pos >= 0 & car_next_pos <= 9);
            
                future_sum = 0;

                % possible values for road or junk in row 3
                for new_road = 0:4
                    for new_junk = 0:4

                        % 5 possibilities for the new road, and 5
                        % possibilities for the new junk
                        T = 1/(5*5);

                        % Possible next state
                        % i1=next_road + (i2=car_next_pos*5) + (i3=next_junk*50) + (i4=new_road*250) + (i5=new_junk*1250)
                        next_state = i4 + (car_next_pos*5) + (i5*50) + (new_road*250) + (new_junk*1250);

                        % Add all possible future rewards

                        if(indexesFromState(next_state)~=-1)
                            future_sum = future_sum + (T*V_star(next_state+1));
                        end

                    end
                end

                % Record reward
                Q_table(state+1, 4-action) = R(state,Rg,Ro,Rj) + gamma * future_sum;
                V_star(state+1) = max(Q_table(state+1, :));

            end
        end
    end  
end;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% TO DO:
% Once the value iteration is done, update the policy!
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for state=0:6249
    
    [val index] = max(Q_table(state+1,:));
    Policy(state+1) = index;
    
end

return;


function [r]=R(s1,Rg,Ro,Rj)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% TO DO:
%%
%% In class, we defined this as R(s,a) which is the
%% reward resulting from taking action a at state s.
%%
%% This is equivalent to evaluating R() at s', the
%% state that results from action a and initial state s.
%% So this function takes only as input s' (here called
%% s1) and the rewards attached to various events.
%%
%% As a reminder, 
%%  Rg is the reward for staying on the road,
%%  Ro is the reward for going off-road,
%%  Rj is the reward for hitting junk
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

[i1,i2,i3,i4,i5]=indexesFromState(s1);

if(i2==(i1+i3))
    r = Rj;
elseif(i2<(i1+1) | i2>(i1+4))
    r = Ro;
else
    r = Rg;
end
    
return;  
 
%% Helpful functions below which you do not need to change!
function [i1,i2,i3, i4, i5]=indexesFromState(s)
%% Returns the components i1 through i5 from the state index s

 i5 = floor(s/1250);
 i4 = floor((s-(i5*1250))/250);
 i3 = floor((s-(i5*1250)-(i4*250))/50);
 i2 = floor((s-(i5*1250)-(i4*250)-(i3*50))/5);
 i1 = s-(i5*1250)-(i4*250)-(i3*50)-(i2*5);

 
 % Check that the state is valid (the road in current and next row
 % must be within one block distance)
 if (abs(i4-i1)>1)
     i1=-1;
     i2=-1;
     i3=-1;
     i4=-1;
     i5=-1;
 end;

return;

