#ifndef QVLEARNING_CPP
#define QVLEARNING_CPP

#include "QVlearning.hpp"

QVlearning::QVlearning(double delta, double probability,  double var, char * path1, char * path2)
{
	char neatname[] = "NEAT";
	char ruta[] = "./NEAT_organisms/";

	cppn_neat = new Population(path1, path2, neatname, ruta);

	this->delta = delta;
	this->probability = probability;
	this->var = var;
}

QVlearning::~QVlearning()
{

}

vector < double > QVlearning::Eval(vector < double > state, vector < vector < double > > actions)
{
	int n_actions = (int)actions.size();
	int length_state = (int)state.size();
	vector < double > next_state (length_state, 0.0);
	double MQ = 0;
	int index = 0;

	for (int i = 0; i < n_actions; i++)
	{
		vector < double > inputs;
		double Q;

		inputs.insert(inputs.end(), state.begin(), state.end());
		inputs.insert(inputs.end(), actions.at(i).begin(), actions.at(i).end());

		Q = (cppn_neat->champion.eval(inputs)).at(0);

		if (Q > MQ)
		{
			MQ = Q;
			index = i;
		}
	}

	if((double)(rand()%101)/100.0 > probability) index = rand()%n_actions;

	for (int i = 0; i < length_state; i++)
	{
		double aux = state.at(i) + delta*actions.at(index).at(i);
		next_state.at(i) = (abs(aux) > 1) ? aux/abs(aux) : aux;
	}

	vector < double > inputs;
	inputs.insert(inputs.end(), state.begin(), state.end());
	inputs.insert(inputs.end(), actions.at(index).begin(), actions.at(index).end());
	TS_input.push_back(inputs);

	return next_state;
}

void QVlearning::SetResult(double result)
{
	TS_output.push_back(result);
}

bool QVlearning::Train()
{
	if ((int)TS_output.size() != (int)TS_input.size())
	{
		clog << "ERROR: Inputs and outputs in training set is not equal" << endl;
		return false;
	}

	int length_TS = (int)TS_output.size();

	for (int i = 0; i < length_TS; i++)
	{
		for (int j = i+1; j < length_TS; j++)
		{
			if (TS_input.at(i) == TS_input.at(j))
			{
				if (TS_output.at(i) < TS_output.at(j))
				{
					TS_output.at(j) += var*TS_output.at(i);
					TS_input.erase(TS_input.begin()+i);
					TS_output.erase(TS_output.begin()+i);
				}
				else
				{
					TS_input.erase(TS_input.begin()+j);
					TS_output.erase(TS_output.begin()+j);					
				}
			}
				
			break;
		}
	}

	srand(time(0));

	for (int i = 0; i < cppn_neat->GENERATIONS; ++i){
		for (int i = 0; i < (int)cppn_neat->organisms.size(); ++i)
		{
			cppn_neat->organisms.at(i).fitness = fitness(cppn_neat->organisms.at(i));
		}
		cppn_neat->epoch();
	}

	return true;
}

double QVlearning::fitness(Genetic_Encoding organism)
{
	int length_TS = (int)TS_output.size();
	double error_sum = 0;

	for (int i = 0; i < length_TS; i++)
		error_sum += abs(TS_output.at(i) - (organism.eval(TS_input.at(i))).at(0));

	return pow(length_TS - error_sum, 2);
}

#endif