%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CSC D84 - Artificial Intelligence - Winter 2020
%
% This function will perform a graph search (UCS) on the
% given input graph.
%
% The graph is passed in as an adjacency list as defined in
% MARS_pathfinding.m (see that script for a description)
%
% You have to implement the search process. You're free to
% do this however you like (i.e. use recursion, or not,
% it's up to you). 
%
% Remember:
% For UCS you need to keep track of the current lowest cost
%  to nodes in the graph (initially this is unknown for all
%  nodes except for s, with cost zero)
% UCS expands next the UNEXPANDED node with lowest cost
%  NO LOOPS ARE ALLOWED - you should NEVER expand a node twice
% You need to keep track of the predecessor for each node
%  so you can build the path at the end.
%
% You can use arrays for the costs and predecessors, or you can use
% whatever other data structure you find helps you do your work.
%
% THE SEARCH FUNCTION MUST RETURN A PATH AS A SEQUENCE OF NODES 
% TO VISIT - the first entry in the path is the start node's index
% and the last entry is the goal node's index.
%
% You can prepend entries to the Path variable below like so:
% Path=[new_index Path];    <- To append, change the order!
%
% Input parameters:
%
% - A graph 'G' of size Nx4 where N is the number of nodes in the graph
% and edges are real valued (the function should not assume the 
% edges are positive, the absence of an edge between two nodes is
% indicated by the value 1e9).
%
% - An array Gid that gives the indexes of the neighbours for each
% node in the graph, as explained in MARS_pathfinding.m
%
% - The index of the start node 's', and the goal node 'g' (we are
% using this function to find a path between two nodes). UCS will
% return the lowest-cost path between 's' and 'g' if edge weights 
% are positive. If there are negative edges, the caveats we
% have discussed apply.
%
% Return values:
%
% The PATH that the function has found, it can be empty if there is no
% path between the nodes.
%
% Starter code: F. Estrada, Jan 2020.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [Path]=graph_search(G,Gid,s,g, REG_SIZE)

Path=[];

%% TO DO:
% Implement Uniform Cost Search here

%% Cost Search
% Initialize queues
Open=[];                % [node_index, cost_from_start, node_parent_index]
Visited=[];             % [node_index, node_parent_index]

% Start at start node
Open = [s 0 -1];

% Loop through nodes until the goal is found
while(~ismember(g, Visited))
    
    curr_node_index = Open(1);
    
    if(~ismember(curr_node_index, Visited))
    
        % Loop through neighbors
        for i=1:4
            
            curr_node_neighbor_index = Gid(curr_node_index, i);
            
            % If neighbor exists and hasn't been checked, check it
            if(curr_node_neighbor_index~=-1 && ~ismember(curr_node_neighbor_index, Open(:,1)))
                
                % Get weight of moving to neighbor
                weight = heuristic(curr_node_index, curr_node_neighbor_index, i, g, G, REG_SIZE);
                %weight = G(curr_node_index,i);
                %if(weight > 0)
                %    weight = weight * weight;
                %end
                
                % Add current neighbor to Open list
                Open = [Open;[curr_node_neighbor_index Open(1,2)+weight curr_node_index]];
            end
        end

        % Add current node and current node parent to Visited list
        Visited = [Visited;Open(1,1) Open(1,3)];
    end
    
    % Remove current node from priority list
    Open = Open(2:end,:);
    % Sort priority list
    Open = sortrows(Open, 2);
end

%% Path Build
% Start with goal node
curr_visited_node=Visited(end,:);

while(~ismember(s, Path))
    % Add current node to path
    Path = [curr_visited_node(1) Path];
    
    % Find parent node
    curr_visited_parent = curr_visited_node(2);
    curr_visited_parent_index = Visited(:,1)==curr_visited_parent;
    curr_visited_node = Visited(curr_visited_parent_index,1:end);
end

return;

%% Heuristic
function h = heuristic(curr_node_index, neighbor_node_index, neighbor_number, goal_index, G, REG_SIZE)

    
	neighbor_x = mod(neighbor_node_index, REG_SIZE);
	neighbor_y = floor(neighbor_node_index / REG_SIZE)+1;
    
	goal_x = mod(goal_index, REG_SIZE);
	goal_y = floor(goal_index / REG_SIZE)+1;
	
    % Euclidean distance to goal
	h = norm([neighbor_x, neighbor_y] - [goal_x, goal_y]);
    
    
    delta = G(curr_node_index, neighbor_number);
    
    
    if(delta > 0)
        delta = delta * delta;
    end
    
    h = h + delta + 1;
return;


