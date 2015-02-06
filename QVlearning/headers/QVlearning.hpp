#ifndef QVLEARNING_HPP
#define QVLEARNING_HPP

#include <NEAT>
#include <ctime>

using namespace ANN_USM;
using namespace std;

class QVlearning
{
	double delta;
	double probability;
	double var;//la wea del factor de las recompenzas pasadas

	vector < vector < double > > TS_input;
	vector < double > TS_output;

	double fitness(Genetic_Encoding organism);

public:

	Population * cppn_neat;

	QVlearning(double delta, double probability, double var, char * path1, char * path2);
	~QVlearning();

	vector < double > Eval(vector < double > state, vector < vector < double > > actions);
	void SetResult(double result);
	bool Train();
	
};

#endif