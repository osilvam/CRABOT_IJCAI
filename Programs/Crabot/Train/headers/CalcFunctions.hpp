#ifndef CALCFUNCTIONS_HPP
#define CALCFUNCTIONS_HPP

#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

double mean(vector < double > numbers);
double mean(vector < int > numbers);
double stdDesviation(vector < double > numbers);
double stdDesviation(vector < int > numbers);
double getDistance(double init[3], double final[3]);

#endif