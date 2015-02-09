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
		inputs.insert(inputs.end(), actions.at(i).begin(), actions.at(i).end());

		Q = (organism.eval(inputs)).at(0);

		if (Q > Max_Q)
		{
			Max_Q = Q;
			index = i;
		}
	}

	if((double)(rand()%101)/100.0 > probability) index = rand()%length_actions;

	for (int i = 0; i < length_state; i++)
	{
		double aux = state.at(i) + delta*actions.at(index).at(i);
		next_state.at(i) = (abs(aux) > 1) ? aux/abs(aux) : aux;
	}

	vector < double > inputs;
	inputs.insert(inputs.end(), state.begin(), state.end());
	inputs.insert(inputs.end(), actions.at(index).begin(), actions.at(index).end());
	temp_input.push_back(inputs);

	return next_state;
}

void QVlearning::SetResult(double result)
{
	temp_output.push_back(result);
}

bool QVlearning::Train()
{
	if ((int)temp_output.size() != (int)temp_input.size())
	{
		clog << "ERROR: Inputs and outputs in training set is not equal" << endl;
		return false;
	}
	
	AddToTrainingSet();

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

void QVlearning::AddToTrainingSet()
{
	int length_TS = (int)TS_input.size();
	int length_temp = (int)temp_input.size();

	for (int i = 0; i < length_temp; i++)
	{
		bool exist = false;

		for (int j = 0; j < length_TS; j++)
		{
			if (temp_input.at(i) == TS_input.at(j))
			{
				exist = true;
				temp_output.at(i) = FUNCTION(temp_output.at(i) + discount*TS_output.at(j));

				if (temp_output.at(i) > TS_output.at(j))
				{					
					TS_output.at(j) = temp_output.at(i);
					break;
				}
			}
		}

		if (!exist)
		{
			temp_output.at(i) = FUNCTION(temp_output.at(i));
			TS_input.push_back(temp_input.at(i));
			TS_output.push_back(temp_output.at(i));
		}
	}

	temp_input.clear();
	temp_output.clear();
}

#endif