#ifndef Quadratot_CPP
#define Quadratot_CPP

#include "Quadratot.hpp"

int sum(int num, int init, int limit)
{
	int result = (num < limit) ? num + 1 : init;
	return result;
}

vector < vector < double > > MakeActions()
{
	int n_legs = N_LEGS;
	int gl = GRA_LIB;
	int gle = GRA_LIB_EXT;	
	int rep[n_legs];

	for(int i = 0; i < n_legs; i++)
		rep[i] = pow(2*gl,i);

	int cont[n_legs];
	int action_leg[n_legs];

	for(int i = 0; i < n_legs; i++)
		cont[i] = action_leg[i] = 0;
	
	double moves[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};

	vector < vector < double > > temp_actions;
	vector < vector < double > > actions;
	
	for(int i = 0; i < pow(2*gl,n_legs); i++)
	{
		for(int j = 0; j < n_legs; j++)
		{
			cont[j]++;
			if(cont[j] == rep[j])
			{
				cont[j] = 0;
				action_leg[j] = sum(action_leg[j],0,2*gl-1);
			}
		}
		vector < double > action;

		for(int j = 0; j < n_legs; j++)
		{
			vector < double > leg (moves[action_leg[j]],moves[action_leg[j]] + sizeof(moves[action_leg[j]])/sizeof(double));
			action.insert(action.end(),leg.begin(),leg.end());
		}
		temp_actions.push_back(action);		
	}

	for(int k = 0; k < (int)temp_actions.size(); k++)
	{
		vector < double > action;

		for(int i = 0; i < gl; i++)
		{
			for(int j = 0; j < n_legs; j++)
			{		
				action.push_back(temp_actions.at(k).at(i + gl*j));
			}
		}

		actions.push_back(action);	
	}	

	if(gle != 0)
	{
		temp_actions = actions;
		actions.clear();
		double act[3] = {-1,0,1};

		for(int j = 0; j < 3; j++)
			for (int i = 0; i < (int)temp_actions.size(); i++)
			{
				vector < double > aux(temp_actions.at(i));
				aux.push_back(act[j]);
				actions.push_back(aux);
			}
	}

	return actions;
}

int main(int argc, char* argv[])
{	
	if(argc < 3)
	{
		cerr << "ERROR: The number of arguments is incorrect" << endl;
		return -1;	
	}

	RobotSimulator * simulator = new RobotSimulator();
	SimFiles * files = new SimFiles();

	int n_iteration = atoi(argv[1]);
	
	simulator->simStart();
	// ============= VREP INITIALIZATIONS ============= //			

	vector < Joint * > joints;
	vector < CollisionObject * > body_parts;
	Dummy * center_dummy = new Dummy(simulator, (char*)"center");

	double max_angle_limit[] = MAX_ANGLE_LIMIT;
	double min_angle_limit[] = MIN_ANGLE_LIMIT;

	for(int i = 0; i < N_LEGS*GRA_LIB + GRA_LIB_EXT; i++)
	{
		stringstream joint;
		joint << "joint" << i << "#";
		joints.push_back(new Joint(simulator ,(char*)joint.str().c_str(), max_angle_limit[i], min_angle_limit[i], (char*)"SCALE"));
	}

	for(int i = 0; i < 16; i++)
	{
		stringstream body_part;
		body_part << "Collision" << i << "#";
		body_parts.push_back(new CollisionObject(simulator, (char*)body_part.str().c_str()));
	}
	
	// ================================================ //

	// ========== QVLEARNING INITIALIZATIONS =========== //

	vector < double > state((int)(N_LEGS*GRA_LIB + GRA_LIB_EXT),0.0);
	vector < vector < double > > actions = MakeActions();

	double delta = 0.05;
	double probability = 0.9;
	double discount = 0.8;

	QVlearning * qvlearning = new QVlearning(delta, probability, discount, argv[2], argv[3]);

	// ================================================ //

	
	if (simulator->simGetConnectionId() != -1)
	{
		for(int p = 0; p < n_iteration; p++)
		{
			double sim_time = 0;
			bool flag = true;
			double pass_pos[3] = {0.0, 0.0, 0.0};
			double next_pos[3];

			stringstream message1, message2;

			for(int i = 0; i < (int)joints.size(); i++)
				state.at(i) = joints.at(i)->getJointInitialPosition();

			//Revisar - esto es para evitar un problema de error en lectura de primeros valores.
			
			center_dummy->getPosition(-1, NULL);
			center_dummy->getPosition(-1, NULL);
			center_dummy->getOrientation(-1, NULL);
			center_dummy->getOrientation(-1, NULL);
			//

			files->openNewRobotPositionFile(p);

			clog << "\t\tIteration #" << p << endl;

			message1 << "Iteration #" << p;
			simulator->simAddStatusbarMessage((char*)message1.str().c_str() , simx_opmode_oneshot_wait);

			simulator->simStartSimulation(simx_opmode_oneshot_wait);

			usleep(100000);

			while(sim_time < TIME_SIMULATION)
			{					
				state = qvlearning->Eval(state,actions);

				simulator->simPauseCommunication(1);

				for(int i = 0; i < (int)joints.size(); i++)
					joints.at(i)->setJointPosition(state.at(i));

				simulator->simPauseCommunication(0);				

				for(int i = 4; i < (int)body_parts.size(); i++)
				{
					if(body_parts.at(i)->getCollisionState() != 0)
					{
						flag = false;
						break;
					}
				}

				usleep((int)(DELTA_T*1000000.0));
				sim_time += DELTA_T;

				if (flag)
				{
					center_dummy->getPosition(-1, next_pos);
					qvlearning->SetResult(getDistance(pass_pos, next_pos));

					for(int i = 0; i < 3; i++)
						pass_pos[i] = next_pos[i];

					files->addFileRobotPosition(center_dummy,sim_time);
				}					
				else
				{
					qvlearning->SetResult();
					break;
				}				
			}

			simulator->simStopSimulation(simx_opmode_oneshot_wait);

			if(flag)
			{					
				double init_pos[3] = {0.0, 0.0, 0.0};
				double final_pos[3];
				center_dummy->getPosition(-1, final_pos);
				double fitness = getDistance(init_pos, final_pos);
				
				clog << "Fitness : " << fitness << endl << endl;
			
				message2 << "FITNESS : " << fitness;
				simulator->simAddStatusbarMessage((char*)message2.str().c_str() , simx_opmode_oneshot_wait);

				files->addFileFitness(fitness, p);
			}

			files->closeRobotPositionFile();

			qvlearning->Train();

		}		
	}
		
	simulator->simFinish();

	delete(simulator);
	delete(qvlearning);
	delete(files);
	
	return(0);

}



#endif

