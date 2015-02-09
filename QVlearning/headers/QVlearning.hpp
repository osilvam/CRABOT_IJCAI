#ifndef QVLEARNING_HPP
#define QVLEARNING_HPP

#include <NEAT>
#include <ctime>
#include <cmath>

#define FUNCTION(X) (double)(atan(X));

using namespace ANN_USM;
using namespace std;

class QVlearning
{
	double delta;
	double probability;
	double discount;//la wea del factor de las recompenzas pasadas

	vector < vector < double > > TS_input;
	vector < double > TS_output;
	vector < vector < double > > temp_input;
	vector < double > temp_output;

	Genetic_Encoding organism;

	char * user_def;
	char * genetic_encoding; 

	double Fitness(Genetic_Encoding organism);
	void AddToTrainingSet();

public:

	QVlearning(double delta, double probability, double discount, char * path1, char * path2);
	~QVlearning();

	vector < double > Eval(vector < double > state, vector < vector < double > > actions);
	void SetResult(double result);
	bool Train();
	
};

#endif