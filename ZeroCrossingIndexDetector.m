%RETURNS INDICES OF ZERO CROSSINGS IN THE REAL SIGNAL

% Please feel free to use the code in your research and development works.
% We would appreciate a citation to the paper below when this code is helpful in obtaining results in your future publications.

% Publication for citation:
%(1) Abu Sajana R., R. Subramanian, P. Vijay Kumar, Syam Krishnan, Bharadwaj Amrutur, Jeena Sebastian, Malati Hegde, S. V. R. Anand,
% "A Low-Complexity Algorithm for Intrusion Detection in a PIR-Based Wireless Sensor Network,"
% In 5th International Conference on Intelligent Sensors, Sensor Networks and Information Processing (ISSNIP), pages 337–342, Dec 2009.

%(2) The SmartDetect WSN Team, "SmartDetect: An efficient WSN implementation for intrusion detection,"
% Communication Systems and Networks (COMSNETS), 2010 Second International Conference on, Bangalore, 2010, pp. 1-2.

% We suggest the user refer to the publication for implementation details.

clear all; close all; clc

% Load Real Data
load C2v0800016.dat; A=C2v0800016(:,2);
% Signal region of interest
A=A(42300:164100);
% remove DC in signal
A=A-mean(A);

k=1;

for i=1:length(A)-1
    if((A(i)<0 && A(i+1)>=0)||(A(i)>0 && A(i+1)<=0))
        index(k)=i;
        k=k+1;
    end;
end;

plot(A,'LineWidth',2);
hold on;
plot(index,0,'*','LineWidth',2,'MarkerSize',10);
title('Indices of Zero Crossings in the Signal','FontSize',25,'FontWeight','bold')
xlabel('Sample Index','FontSize',19,'FontWeight','bold')
