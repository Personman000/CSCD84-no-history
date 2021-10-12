%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  CSC D84 - Artificial Intelligence - UTSC
%
%  We will practice solving a CSP using backtracking 
%  search. The basic search process will be implemented
%  recursively, and the function will keep track of how
%  many times it's called until a solution is found.
%
%  We will then implement the various variable selection
%  strategies discussed in lecture, and check how much 
%  of a reduction in search we get from those.
%
%  The task at hand is sudoku. The input is a partially
%  completed sudoku puzzle (which is just a 9x9 array
%  with numbers 1-9. Unassigned squares are marked with 0)
%
%  Your task is to implement backtracking search, then
%  to enhance it to implement variable selection 
%  methods as discussed in lecture.
%
%  [sol]=SuDoKu(input)
%  
%  input - a 9x9 array containing the initial puzzle
%  sol - a 9x9 array with the completed puzzle
%
%  For input puzzles, have a look here:
%   https://www.sudoku-puzzles-online.com/index.php
%
%  Starter code: F. Estrada, Jan 2020.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [sol]=SuDoKu(input)

    persistent call_count;
        
    if isempty(call_count)
        call_count=0;
    end;
    call_count=call_count+1;
        
    % This is a recursive solver for a sudoku.
    % The simplest version of it chooses a variable,
    % then tries all values left to assign to it
    % that DO NOT BREAK any constraints.
    
    % It assigns that variable each of these values,
    % one at a time, and then tries to recursively
    % solve the smaller resulting sudoku

    % FIRST: Understand the vanilla solver below.
    % Run it and see how long it takes.
    
    idx=find(input(:)==0);      % Check for remaining unassigned cells
    if (isempty(idx))
        fprintf('Solved! SuDoKu() calls = %d\n',call_count);
        sol=input;
        return;
    end;
    
    remain = length(idx);
    idy = zeros(remain, 3);
    
    % Loop through all nodes
    for index = 1:remain
        id=idx(index);

        % Get node index
        [i,j]=ind2sub(size(input),id);
        ii=floor((i-1)/3);
        jj=floor((j-1)/3);
        
        i_gr_start=1+(ii*3);
        i_gr_end=3+(ii*3);
        
        j_gr_start=1+(jj*3);
        j_gr_end=3+(jj*3);

        gr=input(i_gr_start:i_gr_end,j_gr_start:j_gr_end);    % Get current node subgrid
        row=input(i,:);                                       % Get current node row
        col=input(:,j);                                       % Get current node column
        
        subrow=[row(1:j_gr_start-1) row(j_gr_end+1:end)];     % Use subrow to avoid recounts in subgrid
        subcol=[col(1:i_gr_start-1) col(i_gr_end+1:end)];     % Use subcol to avoid recounts in subgrid

        taken=unique(union(union(row(find(row>0)),col(find(col>0))),gr(find(gr>0))))';                               % Get indexes of current node's non 0 values
        left=length(unique(setdiff([1 2 3 4 5 6 7 8 9],taken)));                                                     % Get indexes of current node's remaining possible values
        constraints=length(subrow(find(subrow==0))) + length(subcol(find(subcol==0))) + length(gr(find(gr==0))) - 1; % Get indexes of current node's constraints (0 values, -1 to exclude current node)

        node_states(index,:) = [id left constraints];         % Store values
    end
    
    least_vars_indices=find(node_states(:,2) == min(node_states(:,2)));
    least_vars=node_states(least_vars_indices,:);
    most_cons_indices=find(least_vars(:,3) == max(least_vars(:,3)));
    most_cons=least_vars(most_cons_indices,:);
    
    % If we can't use any more values, we are stuck. Return empty solution
    if(most_cons(1,2)<1)
        sol=[];
        return;
    end
    
    id=most_cons(1,1);                  % Get index of the found cell
    [i,j]=ind2sub(size(input),id);      % Get location [i,j] of this cell in the input array
    ii=floor((i-1)/3);                  % Figure out which 3x3 sub-grid it's in
    jj=floor((j-1)/3);
    
    % Find the set of remaining values for this cell
    i_gr_start=1+(ii*3);
    i_gr_end=3+(ii*3);

    j_gr_start=1+(jj*3);
    j_gr_end=3+(jj*3);

    gr=input(i_gr_start:i_gr_end,j_gr_start:j_gr_end);      % get the subgrid   
    row=input(i,:);                                     % get the row
    col=input(:,j);                                     % get the column

    % make a list of values that are already assigned in the row, column, or sub-grid
    taken=unique(union(union(row(find(row>0)),col(find(col>0))),gr(find(gr>0))))';
    
    % And then determine which values are left that we have to try
    left=unique(setdiff([1 2 3 4 5 6 7 8 9],taken));

    % Pick the least constraining value by looping through all cells in the
    % subgrid and checking for which value is least constraining to them
    all_left=[];
    for i_gr_index=i_gr_start:i_gr_end
        for j_gr_index=j_gr_start:j_gr_end

            % Skip chosen cell and non-zero cells
            if((i_gr_index~=i || j_gr_index~=j) && sub2ind(size(input),i_gr_index,j_gr_index)>0)
                
                curr_row=input(i_gr_index,:);                                     % get the current row
                curr_col=input(:,j_gr_index);                                     % get the current column

                % Get current cell's constrained values
                curr_taken=unique(union(union(curr_row(find(curr_row>0)),curr_col(find(curr_col>0))),gr(find(gr>0))))';

                % Get possible values for current cell
                curr_left=unique(setdiff([1 2 3 4 5 6 7 8 9],curr_taken));
                
                % From the current cell's remaining possible values, prune
                % any values that the chosen cell can't use
                all_left=[all_left intersect(curr_left,left)];
            end
        end
    end
    
    % If all neighbors have similar possible values, just use any
    if(isempty(all_left))
        all_left=left;
    end
    
    min_left_indices=accumarray(all_left(:),1);
    min_left_indices(min_left_indices==0)=inf;
    
    % Attempt to recursively solve smaller sudoku puzzle
    sol=[];
    % Loop through all the possible min values
    while(min(min_left_indices)~=inf)
        [min_left_index, min_left_value]=min(min_left_indices); % Use the smallest possible value
        min_left_indices(min_left_value)=inf;                   % Remove the chosen value from the list

        input(id)=min_left_value;                               % Plug the value into the board
        
        sol=SuDoKu(input);                                      % Solve the smaller board
        
        if (~isempty(sol))                                      % If the recursive call solved it, return!
            return;
        end;
    end
        
        
    % attempt to recursively solve the smaller sudoku puzzle.
    % If we run out of values to try without finding a solution, return [] (empty)
    
%     sol=[];                         % No solution just yet
%     for i=1:length(left)
%         input(id)=left(i);
%         sol=SuDoKu(input);
%         if (~isempty(sol))          % If the recursive call solved it, return!
%             return;
%         end;
%     end;
    
    %% TO DO:
    %% Modify the solver provided to implement the variable ordering criteria
    %% we discussed in lecture:
    %% 1 - Choose first the variable with fewest remaining values
    %% 2 - If (1) has a tie, choose variable that intervenes in more active constraints
    %% 3 - Once you have chosen a variable, select the least constraining value to try first!
    %%
    %% You have to decide how to keep track of 
    %%   # of valid values left for each unassigned cell
    %%   # of active constraints for each unassigned cell
    %%
    %% And you need to think about how to check which values are least constraining.
    %% since there may be many, you'll need to sort them from least constraining to
    %% most constraining!
    %%
    %% BUT! the goal is not to make the most efficient data structure for these things!
    %% (that's not what Matlab's for, you can do that in C if you like). Here, we only
    %% care about *reduction in the number of calls to SuDoKu()* which tells us something
    %% important about the size of the search tree! 
    %%
    %% It is assumed you can then make your data structures fast in a language designed
    %% to do so, and the improved algorithm (which uses the variable selection process
    %% we just described) will beat the 'vanilla' version because of the reduction in
    %% the number of nodes that need to be expanded to reach the full solution.
    %%
    %% So - don't spend time writing clever data structures, just get this to work, and
    %% see what you can tell about how much it saves in terms of calls to SuDoKu().
        
return;
