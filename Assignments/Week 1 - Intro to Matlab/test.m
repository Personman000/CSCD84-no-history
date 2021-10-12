i=rand();   % Get a random number
if (i<.5)
    if (i<.25)
        fprintf('i is less than .25\n');     % BTW, print statements and text strings use single ' in Matlab
    else
        fprintf('i is between .25 and .5\n');
    end;        % Matches the 'if (i<.25...
else
    if (i<.75)
        fprintf('i is between .5 and .75\n');
    else
        fprintf('i is greater than .75\n');
    end;        % Matches the 'if (i<.75...
end;    % Matches the 'if (i<.5...