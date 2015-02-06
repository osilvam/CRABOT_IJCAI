#ifndef Crabot_CPP
#define Crabot_CPP

#include "Crabot.hpp"

int main(int argc, char* argv[])
{	
	if(argc < 3)
	{
		cerr << "ERROR: The number of arguments is incorrect" << endl;
		return -1;	
	}

	RobotSimulator * simulator = new RobotSimulator();
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
		joint << "joint" << i+6;
		joints.push_back(new Joint(simulator ,(char*)joint.str().c_str(), max_angle_limit[i], min_angle_limit[i], (char*)"SCALE"));
	}

	for(int i = 0; i < 13; i++)
	{
		stringstream body_part;
		body_part << "Collision" << i << "#";
		body_parts.push_back(new CollisionObject(simulator, (char*)body_part.str().c_str()));
	}
	
	// ================================================ //

	// ========== QVLEARNING INITIALIZATIONS =========== //

	vector < double > state (N_LEGS*GRA_LIB + GRA_LIB_EXT,0.0);

	//FALTA ACTIONS

	QVlearning * qvlearning = new QVlearning(delta, probability, var, argv[2], argv[3]);

	// ================================================ //

	
	if (simulator->simGetConnectionId() != -1)
	{
		for(int p = 0; p < n_iteration; p++)
		{
			double sim_time = 0;
			bool flag = true;
			stringstream message1, message2;

			for(int i = 0; i < (int)joints.size(); i++)
				state.at(i) = joints.at(i)->getJointInitialPosition();

			//Revisar - esto es para evitar un problema de error en lectura de primeros valores.
			center_dummy->getPosition(-1);
			center_dummy->getPosition(-1);
			center_dummy->getOrientation(-1);
			center_dummy->getOrientation(-1);
			//

			message1 << "Iteration #" << p;
			simulator->simAddStatusbarMessage((char*)message1.str().c_str() , simx_opmode_oneshot_wait);

			simulator->simStartSimulation(simx_opmode_oneshot_wait);

			usleep(100000);

			while(sim_time < TIME_SIMULATION)
			{						
				state = qvlearning->Eval(state,actions)

				simulator->simPauseCommunication(1);

				for(int i = 0; i < (int)joints.size(); i++)
					joints.at(i)->setJointPosition(state.at(i));

				simulator->simPauseCommunication(0);				

				for(int i = 6; i < (int)body_parts.size(); i++)
				{
					if(body_parts.at(i)->getCollisionState() != 0)
					{
						flag = false;
						break;
					}
				}

				if (flag)
					qvlearning->SetResult(center_dummy->getPosition(-1));
				else
				{
					qvlearning->SetResult(-10);
				}

				usleep((int)(DELTA_T*1000000.0));
				sim_time += DELTA_T;
			}

			simulator->simStopSimulation(simx_opmode_oneshot_wait);

			if(flag)
			{					
				clog << "Traveled distance : " << center_dummy->getPosition(-1) << endl;
				clog << "Fitness: " << endl;
			
				message2 << "FITNESS : " << /*ACA VA EL FITNESS*/;
				simulator->simAddStatusbarMessage((char*)message2.str().c_str() , simx_opmode_oneshot_wait);
			}
			else
			{	
				hyperneat->HyperNeatFitness(FAILED_FITNESS, p);
			}
		}		
	}
		
	simulator->simFinish();

	delete(simulator);
	delete(qvlearning);
	
	return(0);

}

#endif