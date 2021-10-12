%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% CSC D84 - Winter 2020 - UTSC
%
% Driving simulator for reinforcement learning.
%
% Call this once you have a policy (or feed it a
% policy of all 2's which makes the car stay on
% the same lane)
%
% Then see how far your car goes :)
%
% There's NOTHING for you to complete in this
% script, all your work will be in RL_Driving.m
%
% Script: F. Estrada, Feb. 2020
%
% function []=DrivingSim(Policy,jnkprob,ptime)
%   Policy: A 6250x1 array with optimal actions
%   jnkprob: Probability of junk on the road in [0,1]
%   ptime: pause time between updates (so you can see
%          what's going on if you have a fast computer)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function []=DrivingSim(Policy, jnkprob, ptime)

if (jnkprob>1) jnkprob=1; end;
if (jnkprob<0) jnkprob=0; end;

% Generate initial road map - 10 rows, car at the 
% bottom. All the same before we start driving.

stuff=zeros(10,2);      % For each row we need road position, and junk position
for i=1:10
 stuff(i,1)=2;          % Let's start at the middle of the road
 stuff(i,2)=0;          % No junk
end;

carIdx=3;               % Car is initially at the left lane

roadIm=fillRoadIm(stuff,carIdx);   % Makes an image from the stuff in the road and
                                   % the car location
                                   
figure(1);clf;image(roadIm);axis image;axis off;

%% Driving loop
crashed=0;
while(~crashed)
    
    % Get current state, and next action from policy
    i1=stuff(10,1);
    i2=carIdx;
    i3=stuff(10,2);
    i4=stuff(9,1);
    i5=stuff(9,2);
    s=i1+(i2*5)+(i3*50)+(i4*250)+(i5*1250);

    a=Policy(s+1);      % Mind the +1
    if (a<1||a>3)
        fprintf(2,'Invalid action from policy, state %d, action %d (must be in 1-3)\n',s,a);
        a=1;
    end;
    
    % Next carIdx given action
    carIdx=carIdx+(a-2);
    
    if (carIdx<0||carIdx>9)
        crashed=1;
        fprintf(2,'The car left the driving field!\n');
        break;
    end;
    
    % Get new row for top of the driving field
    disp=round(randn);
    if (disp<0&&stuff(1,1)>0)
        nr=stuff(1,1)-1;
    else if (disp>0&&stuff(1,1)<4)
            nr=stuff(1,1)+1;
        else
            nr=stuff(1,1);
        end;
    end;
    
    % Junk?
    if (rand<=jnkprob);
        dice=rand;
        if (dice<.25) jk=1;
        else if (dice<.5) jk=2;
             else if (dice<.75) jk=3;
                  else jk=4;
                  end;
             end;
        end;
    else
        jk=0;
    end;
    
    % Displace stuff
    stuff(2:end,:)=stuff(1:end-1,:);
    stuff(1,:)=[nr jk];
    
    % Render the updated field
    roadIm=fillRoadIm(stuff,carIdx);
    
    % Check for crashing
    if (carIdx<stuff(10,1)+1||carIdx>stuff(10,1)+4||(carIdx==stuff(10,1)+stuff(10,2)&&stuff(10,2)>0))
        crashed=1;
        fprintf(2,'The car left the road, or hit junk!\n');
    end;

    figure(1);image(roadIm);axis image;drawnow;
    pause(ptime);
end;

for i=1:5
    roadIm=255-roadIm;
    figure(1);image(roadIm);axis image;drawnow;
end;
                                   
return;

function [roadIm]=fillRoadIm(stuff,carIdx)
 %% Generate an image for the road from the stuff array and the car position
 persistent bz;
 persistent cr;
 persistent rd1;
 persistent rd1a
 persistent rd2;
 persistent rd2a;
 persistent rd3;
 persistent vg1;
 persistent vg2;
 persistent vg3;
 persistent vg4;
 persistent veggieField;

 if (isempty(bz))
  load DSim_images.mat;
  bz=bazofia;
  cr=carrito;
  rd1=road1;
  rd1a=road1a;
  rd2=road2;
  rd2a=road2a;
  rd3=road3;
  vg1=veg1;
  vg2=veg2;
  vg3=veg3;
  vg4=veg4;
  veggieField=1+round(rand(10,10)*3);
 end;
 
 roadIm=zeros([10*size(bz,1) 10*size(bz,2) 3]);

 for i=1:10
   row=zeros([size(bz,1) 10*size(bz,2) 3]);

   rdS=stuff(i,1)+1;    % Road start index in image blocks
   
   % Left side veggies
   for j=1:rdS
     dc=veggieField(i,j);
     if (dc==1) v=vg1;
     else if (dc==2) v=vg2;
         else if (dc==3) v=vg3;
             else v=vg4;
             end;
         end;
     end;
     row(:,1+((j-1)*size(bz,2)):(j*size(bz,2)),:)=v;  
   end;
   
   % Road
   if (mod(i,2)==0)
    row(:,1+(rdS*size(bz,2)):((rdS+1)*size(bz,2)),:)=rd1;  
    row(:,1+((rdS+1)*size(bz,2)):((rdS+2)*size(bz,2)),:)=rd2;  
    row(:,1+((rdS+2)*size(bz,2)):((rdS+3)*size(bz,2)),:)=rd2;  
    row(:,1+((rdS+3)*size(bz,2)):((rdS+4)*size(bz,2)),:)=rd3;  
   else
    row(:,1+(rdS*size(bz,2)):((rdS+1)*size(bz,2)),:)=rd1a;  
    row(:,1+((rdS+1)*size(bz,2)):((rdS+2)*size(bz,2)),:)=rd2a;  
    row(:,1+((rdS+2)*size(bz,2)):((rdS+3)*size(bz,2)),:)=rd2a;  
    row(:,1+((rdS+3)*size(bz,2)):((rdS+4)*size(bz,2)),:)=rd3;  
   end;
   
   % Right side veg
   for j=rdS+5:10
     dc=veggieField(i,j);
     if (dc==1) v=vg1;
     else if (dc==2) v=vg2;
         else if (dc==3) v=vg3;
             else v=vg4;
             end;
         end;
     end;
     row(:,1+((j-1)*size(bz,2)):(j*size(bz,2)),:)=v;  
   end;
       
   % Junk
   if (stuff(i,2)>0)
    row(:,1+((rdS+stuff(i,2)-1)*size(bz,2)):((rdS+stuff(i,2))*size(bz,2)),:)=bz;  
   end;
  
   % car
   if (i==10)
     row(:,1+(carIdx*size(bz,2)):(carIdx+1)*size(bz,2),:)=cr;
   end;
   
   % Stuff row into image
   roadIm(1+((i-1)*size(bz,1)):i*size(bz,1),:,:)=row;

 end;   % End for i

 % Update veggieField
 veggieField(2:end,:)=veggieField(1:end-1,:);
 veggieField(1,:)=1+round(rand(1,10)*3);
 roadIm=uint8(roadIm);
 
return;

