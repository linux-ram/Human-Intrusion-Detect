%PIR SENSOR MODELED WITH INTRUDER DATA IN THE ABSENCE OF CLUTTER
%DATA RECORDED WITH PIR SENSOR PLACED 1 METER ABOVE THE GROUND LEVEL

% Please feel free to use the code in your research and development works.
% We would appreciate a citation to the paper below when this code is helpful in obtaining results in your future publications.

% Publication for citation:
%(1) Abu Sajana R., R. Subramanian, P. Vijay Kumar, Syam Krishnan, Bharadwaj Amrutur, Jeena Sebastian, Malati Hegde, S. V. R. Anand,
% "A Low-Complexity Algorithm for Intrusion Detection in a PIR-Based Wireless Sensor Network,"
% In 5th International Conference on Intelligent Sensors, Sensor Networks and Information Processing (ISSNIP), pages 337–342, Dec 2009.

%(2) The SmartDetect WSN Team, "SmartDetect: An efficient WSN implementation for intrusion detection,"
% Communication Systems and Networks (COMSNETS), 2010 Second International Conference on, Bangalore, 2010, pp. 1-2.

% We suggest the user refer to the publication for implementation details.

clc; clear; close;

% Load Real Data
load C2v0800057.dat; b=C2v0800057(:,2);
% Decimation by a factor of 500
b=b(1:500:length(b));
% Signal region of interest
b=b(32:361);

d=2; v=0.84; theta=90*pi/180;

phi=(pi/180)*[20 16 13 13 11 17 18];

%%%Use to intruder_signal(-t) => intruder_signal(t)%%%
%
% for i=0:length(phi)-1
%     new_phi(i+1)=phi(length(phi)-i);
% end;
% phi=new_phi
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

del=[phi(2)+phi(3)+(phi(4)/2)
    phi(3)+(phi(4)/2)
    (phi(4)/2)
    0
    (phi(4)/2)
    phi(5)+(phi(4)/2)
    phi(6)+phi(5)+(phi(4)/2)];


D_vector=d*[((sin(del(1)+phi(1))/sin(pi-theta-phi(1)-del(1)))-(sin(del(1))/sin(pi-theta-del(1))))
    ((sin(del(2)+phi(2))/sin(pi-theta-phi(2)-del(2)))-(sin(del(2))/sin(pi-theta-del(2))))
    ((sin(del(3)+phi(3))/sin(pi-theta-del(3)-phi(3)))-(sin(del(3))/sin(pi-theta-del(3))))
    ((sin(phi(4)/2)/sin(theta-phi(4)/2))+(sin(phi(4)/2)/sin(pi-theta-phi(4)/2)))
    ((sin(phi(5)+del(5))/sin(theta-phi(5)-del(5)))-(sin(del(5))/sin(theta-del(5))))
    ((sin(del(6)+phi(6))/sin(theta-phi(6)-del(6)))-(sin(del(6))/sin(theta-del(6))))
    ((sin(del(7)+phi(7))/sin(theta-phi(7)-del(7)))-(sin(del(7))/sin(theta-del(7))))];

T=D_vector/v;

t1=0:0.02:T(1);
t2=0:.02:T(2);
t3=0:0.02:T(3);
t4=0:0.02:T(4);
t5=0:0.02:T(5);
t6=0:0.02:T(6);
t7=0:0.02:T(7);

x=[sin((5*pi*t1/(2*T(1)))) sin((2*pi*t2/T(2))+(pi/2)) sin((2*pi*t3/T(3))+(pi/2))...
    sin((2*pi*t4/T(4))+(pi/2)) sin((2*pi*t5/T(5))+(pi/2)) sin((2*pi*t6/T(6))+(pi/2))...
    sin((5*pi*t7/(2*T(7)))+(pi/2))];

y=-(1.5*x+1.5)+3;

figure(1)
% Plot Analytical Model Waveform
plot(y,'LineWidth',2);
hold on;
% Plot Real Data
plot(b,'r','LineWidth',2);
axis([0 length(y) 0 3])
title('Intruder Signature - Analytical Model vs Real Data','FontSize',25,'FontWeight','bold')
xlabel('Amplitude (volts)','FontSize',19,'FontWeight','bold')
