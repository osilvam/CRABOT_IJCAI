#ifndef CALCFUNCTIONS_CPP
#define CALCFUNCTIONS_CPP

#include "CalcFunctions.hpp"

double mean(vector < double > numbers)
{	
	if((int)numbers.size() == 0) return 0.0;

	double average = 0;

	for(int i = 0; i < (int)numbers.size(); i++)
		average += numbers.at(i);
	
	return (double)average/numbers.size();
}

double mean(vector < int > numbers)
{	
	if((int)numbers.size() == 0) return 0.0;

	double average = 0;

	for(int i = 0; i < (int)numbers.size(); i++)
		average += numbers.at(i);
	
	return (double)average/numbers.size();
}

double stdDesviation(vector < double > numbers)
{
	if((int)numbers.size() == 0) return 0.0;

	double average = mean(numbers);
	double sum = 0;

	for(int i = 0; i < (int)numbers.size(); i++)
		sum += pow(numbers.at(i)-average,2);
	
	return sqrt(sum/numbers.size());
}

double stdDesviation(vector < int > numbers)
{
	if((int)numbers.size() == 0) return 0.0;

	double average = mean(numbers);
	double sum = 0;

	for(int i = 0; i < (int)numbers.size(); i++)
		sum += pow(numbers.at(i)-average,2);
	
	return sqrt(sum/numbers.size());
}

double getDistance(double init[3], double final[3])
{	
	double dx = pow(final[0]-init[0],2);
	double dy = pow(final[1]-init[1],2);
	return sqrt(dx + dy);
}

#endif