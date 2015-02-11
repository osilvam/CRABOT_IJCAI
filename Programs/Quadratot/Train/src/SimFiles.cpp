#ifndef SIMFILES_CPP
#define SIMFILES_CPP

#include "SimFiles.hpp"

SimFiles::SimFiles()
{
	file_fitness.open("simulation_files/fitness.txt");
}

SimFiles::~SimFiles()
{
	file_fitness.close();
}

void SimFiles::addFileFitness(double fitness, int iteration)
{
	file_fitness << iteration << "\t" << fitness << endl;
}

void SimFiles::openNewRobotPositionFile(int iteration)
{
	stringstream file_name;
	file_name << "simulation_files/robot_position/robotPosition_#" << iteration << ".txt";
	file_robot_position.open((char*)file_name.str().c_str());
}

void SimFiles::closeRobotPositionFile()
{
	file_robot_position.close();	
}

void SimFiles::addFileRobotPosition(Dummy * dummy, double simulation_time)
{
	double position[3];
	dummy->getPosition(-1, position);

	file_robot_position << simulation_time;

	for(int i = 0; i < 3; i++)
		file_robot_position << "\t" << position[i];

	file_robot_position << endl;
}

#endif