#ifndef SIMFILES_HPP
#define SIMFILES_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <ROBOTLIB>
#include "CalcFunctions.hpp"

using namespace ANN_USM;
using namespace std;

class SimFiles
{
	ofstream file_fitness;
	ofstream file_robot_position;

public:

	SimFiles();
	~SimFiles();
	
	void addFileFitness(double fitness, int iteration);

	void openNewRobotPositionFile(int iteration);
	void closeRobotPositionFile();
	void addFileRobotPosition(Dummy * dummy, double simulation_time);
};

#endif