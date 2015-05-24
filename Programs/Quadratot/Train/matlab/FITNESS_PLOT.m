function [y] = FITNESS_PLOT()
x = '../bin/simulation_files/fitness.txt';
A = importdata(x, '\t');
g = A(:,1);
m = A(:,2);

figure
hold on

plot(g, m, 'DisplayName', 'Fitness')
xlabel('Generation')
ylabel('Fitness')
legend('show')
end

