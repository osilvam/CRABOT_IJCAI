function [y] = POSITION_PLOT(i)
S0 = '../bin/simulation_files/robot_position/robotPosition_#';
S2 = '.txt';
x = [S0 num2str(i) S2];
A = importdata(x, '\t');

figure

subplot(2,2,1);
plot(A(:,1),A(:,2));
title('X Robot Position at Time')
xlabel('Time [s]')
ylabel('X Robot Position [m]')

subplot(2,2,2);
plot(A(:,1),A(:,3));
title('Y Robot Position at Time')
xlabel('Time [s]')
ylabel('Y Robot Position [m]')

subplot(2,2,3);
plot(A(:,2),A(:,3));
title('Map Robot Position')
xlabel('X Position [m]')
ylabel('Y position [m]')

subplot(2,2,4);
plot(A(:,1), sqrt(A(:,2).*A(:,2)+A(:,3).*A(:,3)));
title('Advance of Robot at Time')
xlabel('Time [s]')
ylabel('Position [m]')

end

