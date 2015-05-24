#ifndef QVLEARNING_CPP
#define QVLEARNING_CPP

#include "QVlearning.hpp"

QVlearning::QVlearning(double delta, double probability,  double discount, char * path1, char * path2)
{	
	this->delta = delta;
	this->probability = probability;
	this->discount = discount;

	int size;

	size = strlen(path1)+1;
	user_def = new char[size];
	strncpy(user_def, path1, size);

	size = strlen(path2)+1;
	genetic_encoding = new char[size];
	strncpy(genetic_encoding, path2, size);	
	
	organism.load(genetic_encoding);
}

QVlearning::~QVlearning()
{

}

vector < double > QVlearning::Eval(vector < double > state, vector < vector < double > > actions)
{
	int length_actions = (int)actions.size();
	int length_state = (int)state.size();
	vector < double > next_state (length_state, 0.0);
	double Max_Q = -1;
	int index = 0;

	for (int i = 0; i < length_actions; i++)
	{
		vector < double > inputs;
		double Q;

		inputs.insert(inputs.end(), state.begin(), state.end());

		vector < double > aux_next_state;
		for (int k = 0; k < length_state; k++)
		{
			double aux = state.at(k) + delta*actions.at(i).at(k);
			aux_next_state.at(i) = (abs(aux) > 1) ? aux/abs(aux) : aux;
		}

		inputs.insert(inputs.end(), aux_next_state.begin(), aux_next_state.end());

		Q = (organism.eval(inputs)).at(0);

		if (Q > Max_Q)
		{
			Max_Q = Q;
			index = i;
		}
	}
	
	if ((double)(rand()%101) > probability*100) index = rand()%length_actions;

	for (int i = 0; i < length_state; i++)
	{
		double aux = state.at(i) + delta*actions.at(index).at(i);
		next_state.at(i) = (abs(aux) > 1) ? aux/abs(aux) : aux;
	}

	vector < double > inputs;
	inputs.insert(inputs.end(), state.begin(), state.end());
	inputs.insert(inputs.end(), next_state.begin(), next_state.end());

	TS_input.push_back(inputs);
	TS_teoric_output.push_back((organism.eval(inputs)).at(0));

	return next_state;
}

void QVlearning::SetResult(double result)
{
	TS_output.push_back(FUNCTION(result + discount*TS_teoric_output.back()));
}

void QVlearning::SetResult()
{
	TS_output.push_back(-1.0);
}

bool QVlearning::Train()
{
	if ((int)TS_output.size() != (int)TS_input.size())
	{
		clog << "ERROR: Inputs and outputs in training set is not equal" << endl;
		return false;
	}

	if ((int)TS_output.size() == 0)
	{
		clog << "The training set is empty and the training can not be realized";
		return false;
	}
	
	srand(time(0));

	char neatname[] = "NEAT";
	char ruta[] = "./NEAT_organisms/";

	Population poblacion(user_def,genetic_encoding,neatname,ruta);

	for (int i = 0; i < poblacion.GENERATIONS; ++i){
		for (int i = 0; i < (int)poblacion.organisms.size(); ++i)
		{
			poblacion.organisms.at(i).fitness = Fitness(poblacion.organisms.at(i));
		}
		poblacion.epoch();
	}

	organism = poblacion.champion;

	TS_output.clear();
	TS_input.clear();
	TS_teoric_output.clear();

	return true;
}

double QVlearning::Fitness(Genetic_Encoding organism)
{
	int length_TS = (int)TS_output.size();
	double error_sum = 0;

	for (int i = 0; i < length_TS; i++)
		error_sum += abs(TS_output.at(i) - (organism.eval(TS_input.at(i))).at(0));

	return pow(length_TS - error_sum, 2);
}

#endif