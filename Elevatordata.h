#include <string>
#include <iostream>
#include <sstream>
#include <ctime>
#include <stdexcpt.h>

using namespace std;
const int Floor_Time = 15;	// maximum time to travel to each floor
const int Wait_time = 10;	// maximum wait time after all passengers delivered
const int load_time = 3;
class Passenger 
{
protected:
	int floor_number;
public:
	Passenger();
	~Passenger();
	int destination();
	int getPassengerCount();
	int loadtime();
	bool elevator_call;
};

Passenger::Passenger()
{
	srand((unsigned int)time(NULL));
	elevator_call = true;
}

Passenger::~Passenger()
{
	// empty destructor
	floor_number = 0;
}

int Passenger::destination()
{
	int rand_floor = (rand()%5)+1;	// assigns a random floor number between 1 and 5
	floor_number = rand_floor;
	return floor_number;
}

int Passenger::loadtime()
{
	int time = 3;
	return time;
}

class Queue 
{
public:
	Queue();
	~Queue();
	Passenger *passenger;
	void MakeEmpty();
	void Enqueue(Passenger pass);
	void Dequeue(Passenger &pass);
	bool isFull() const;
	bool isEmpty() const;
	int maxQueue;
	int front;
	int rear;
};

Queue::Queue()
{
	maxQueue = 9;
	front = maxQueue - 1;
	rear = maxQueue - 1;
	passenger = new Passenger[maxQueue];
}

Queue::~Queue()
{
	delete [] passenger;
	for (int i = 0; i < 8; i++)
	{
	   while (passenger > 0)
	   {
		  delete [i] passenger;
		  passenger = NULL;	// resets each passengers to NULL
	   }
	}
}

void Queue::MakeEmpty()
{
	front = maxQueue - 1;
	rear = maxQueue - 1;
}

bool Queue::isEmpty() const
{
	return (rear == front);
}
bool Queue::isFull() const
{
	return ((rear + 1) % maxQueue == front);
}

void Queue::Enqueue(Passenger pass)
{
	if (isFull() )
	{
		cerr << "Maximum passengers already onboard\n";
		return;
	}
	else 
	{
		rear = (rear + 1) % maxQueue;
		passenger[rear] = pass;
		cout << "Passenger added\n";
	}

}

void Queue::Dequeue(Passenger &pass)
{
	if (isEmpty() )
	{
		cerr << "No passengers to remove.\n";
		MakeEmpty();
	}
	else if (!isEmpty() )
	{
		front = (front + 1) % maxQueue;
		pass = passenger[front];
		cout << "Passenger departed\n";
	}
	
}

struct Elevatordata
{
	int total_passengers;	// on elevator
	int total_passengers_on_floor;
	int total_time;
	int elevator_time;
	int elevator_id;
	int floor_number;
	int pass_floor[5];
};

class Elevator 
{
protected:
	int state, floor, direction;
	unsigned int total_passengers;
	const enum states{stopped, moving, loading, unloading, opening, closing}; 
	const enum directions{up, down, idle};
	const enum elevator_states{occupied, unoccupied};
	const enum floors{Lobby, second, third, fourth, fifth};
	int max_elevators;
	int *elevator_id;
	int passengers;
	int passenger_load_time;
	int passenger_floor[5];	// creates a arry of passengers for each floor
	 int *elevators_b;
	 int *elevator_state;
	 int timep;	// passenger time
	 int elevator_time;
	 int depart_time;
	 int in;
	 Queue tempqueue;
	 Elevatordata * newarray;
	 int occurrence;
	 int Passengers_on_all_floors;
	 int *Passengers_per_elevator;
	 int passenger_destinations[400];
	
public:
	Elevator(int);	// to create the specified number of elevators 
	~Elevator();
	void RunElevator(Passenger *pass);
	int ReturnPassenger();
	int ReturnPassengersFloors();
};

Elevator::Elevator(int elevators)
{
	occurrence = 0;
	max_elevators = elevators;
	elevators_b = new int [max_elevators];
	elevator_id = new int [max_elevators];
	elevator_state = new int [max_elevators];
	Passengers_on_all_floors = 0;
	Passengers_per_elevator = new int [max_elevators];
	for (int i = 0; i < max_elevators; i++){
		elevator_id[i] = 0;	// set all id's to zero*/
	}
	
	if ((elevators_b) == nullptr)
	{
		cout << "Failure Null pointer\n";	// print out error message if memory allocation was unsuccessful
	}
	for (int i = 0; i < 400; i++)
		passenger_destinations[i] = 0;
	cout << "Testing all elevators\n";
	for (int i = 0; i < max_elevators; i++)
	{
		elevators_b[i] = 0;	// initalize each elevator in the building to zero
		Passengers_per_elevator[i] = 0;	// initalize passengers per elvator to zero
		cout << elevators_b[i] << endl;	// display the elevators in the building
	}
	for (int i = 0; i < max_elevators; i++)
	{
		elevator_id[i] = i+1;	// intialize each id's elevator by a linear amount by i + 1
		elevator_state[i] = unoccupied;	// set each elevator state to unoccupied
		cout << "Elevator id #: " << elevator_id[i] << endl;	// display each elevators id in the building
		newarray = new Elevatordata[i];
		cout << "Elevator states: " << elevator_state[i] << ": unoccupied\n";
	}
	//elevator_id = elevators;	// creates the number of elevators in the building
	total_passengers = 0;	// has no passengers on entry
	floor = Lobby;	// 0 or first floor
	state = stopped;
	direction = up;
	elevator_time = 0;
	in = 0;
	tempqueue.MakeEmpty();
	depart_time = 0;

	for ( int i = 0; i < 5; i++)
		passenger_floor[i] = 0;	// passengers on every floor initalizes all passengers to zero
}

Elevator::~Elevator()
{
	delete [] elevators_b;
	delete [] elevator_id;
	delete [] newarray;
	delete [] elevator_state;
	delete [] Passengers_per_elevator;
	
}
void Elevator::RunElevator(Passenger *pass)
{
	do {
	switch (state)
	{
	case stopped: // for when it is stopped at the lobby floor
		if ( floor == Lobby && direction == up) 
		{
			if (pass->elevator_call == true){
			state = loading;
			occurrence++;
			}
		}
		else if (floor == Lobby && direction == idle)
		{
			if (pass->elevator_call == true){
			  state = stopped;
			  direction = up;
			}
			else 
				cout << "No current passengers waiting\n";
			//state = closing;
			//cout << "Blank\n";
		}
		break;
	case loading:
		{
		//passengers = 0;	// passengers on board the elevator
		passenger_load_time = 0;
		if (elevator_state[in] == unoccupied)
			{
				elevator_state[in] = occupied;	// set that elevators state to occupied and then do the following
				cout << "Current elevator " << elevator_id[in] << endl;
				cout << "Current elevators state: " << elevator_state[in] << ". 0 for occupied, 1 for unoccupied"<< endl;
				newarray[in].elevator_id = elevator_id[in];
		for (int i = 0; i < 8; i++)
		{
			if (total_passengers < 8)
			tempqueue.Enqueue(pass[i]);

			timep = pass[i].loadtime();
			passenger_load_time += timep;
			total_passengers++;
		}
		newarray[in].total_passengers += total_passengers;
		state = moving;
		direction = up;
		cout << passenger_load_time <<"s "<< "and total passengers on board " << total_passengers << endl;
		if (in < max_elevators)
		in++;
			}
		if (in == max_elevators)	// in case the incrementer goes beyond the actual number of elevators in the system
		{
			in = 0;	// start over from elevator one
			state = loading;
		}
	
		
		else if ( (elevator_state[in] == occupied) )
				{
			cout << "Checking another elevator.\n";
			if (in < max_elevators)
				in = rand()%max_elevators;
			state = stopped;
				}
		}
	break;
	case moving:
		{
			if (direction == up)
			{
				if (floor <= fourth)
				{
				floor++;	// go to the next floor
				elevator_time += Floor_Time;	// add Floor_time or 15s to the elevator time each instance it goes up a floor
				cout << "Elevator traveling to floor: " << floor+1 << " " << elevator_time << "s" << endl; // time it took to go to that floor IN TOTAL
				}
				
				for ( int i = 0; i < (int)total_passengers; i++)
				{
				if (pass[i].destination() == floor)	// if passengers destination is equal to this floor
					state = unloading;	// switch to unloading state
				}
				
			}
			else if (direction == down)
			{
				if (total_passengers == 0){	// if there are no passengers aboard
					cout << "Waiting....\n"; // simulate a waiting mechanism
					for (int i = 10; i > 0; i--)	
							cout << i << ", ";
					cout << "0.\n";
					elevator_time += 10;}	// add a wait time of ten seconds when the last passenger exits
				for (floor; floor > Lobby; floor--) // while floor is above the Lobby decrement floor
				{
					elevator_time += Floor_Time;	// add the Floor_Time to it each time
				}
				cout << "Total time for a round trip: " << elevator_time+passenger_load_time << "s" << endl;
				direction = idle;
				state = stopped;
				
				
				newarray[in-1].total_time += elevator_time+passenger_load_time;
				newarray[in-1].elevator_time += elevator_time;
				
				if (elevator_state[in - 1] == occupied){
				elevator_state[in - 1] = unoccupied;
				}
				cout << "Elevator " << elevator_id[in-1] << "'s state is now: " << elevator_state[in-1] << endl;
				cout << "Number of occurences: " << occurrence << endl;
				
			}
		}
		break;
	case unloading:
		{
			for (int i = 0; i < (int)total_passengers; i++)
			{
				
				if (pass[i].destination() == floor)	// if a passengers destination is equal to the floor
				{	
					if (total_passengers > 0)
					tempqueue.Dequeue(pass[i]);	// remove that passenger

					total_passengers--;	// decrement passengers on board
					timep = pass[i].loadtime();	// intended as departure time here
					passenger_load_time += timep;	// passenger departure time is added
					if (passenger_floor[floor] < 100)
					{
					passenger_floor[floor]++; // add that passenger on that partiuclar floor
					}
					else 
					for (int i = 0; i < 5; i++)
							{
								if (i == 5){
									if (passenger_floor[i-1] < 100)
										passenger_floor[i-1]++;
								}
								else if (passenger_floor[i+1] < 100)	// if any floors have passengers besides the first that are greater than 100 do the following
									passenger_floor[i+1]++;
							}
					depart_time = pass[i].loadtime() * passenger_floor[floor];	// total departure time is equal to the departure time of a single passenger times the number of passengers on a floor
					cout << passenger_floor[floor] << " Passenger(s) total departure time on floor " << floor+1 <<"\n"<< depart_time <<"s" << endl;
					cout << total_passengers << " passenger(s) still on board."<< endl;
					newarray[in-1].pass_floor[floor] = passenger_floor[floor];
				}
				
				/*if (passenger_floor[i] > 100)
					for (int i = 0; i < 5; i++){
						if ((floor == 4) && (passenger_floor[floor-1] < 100 ))
						passenger_floor[floor-1]++;
					    else if ((floor == 3) && (passenger_floor[floor-1] < 100))
							passenger_floor[floor-1]++;	// add passengers to floor 2
						else if ((floor == 2) && (passenger_floor[floor-1] < 100))
							passenger_floor[floor-1]++;
						else if ((floor == 1) && (passenger_floor[floor-1] < 100))
							passenger_floor[floor-1];
						else
						{
							for (int i = 0; i < 5; i++)
							{
								if ( i < 5){
								if (passenger_floor[i+1] < 100)	// if any floors have passengers besides the first that are greater than 100 do the following
									passenger_floor[i+1]++;
								}
								 
								if (i == 5){
									if (passenger_floor[i-1] < 100)
										passenger_floor[i-1]++;
								}
							}
						}
					}
			}*/
			}
				

			if (total_passengers > 0)	// if there are still passengers on board do the following
			{
				state = moving;	// set state to moving 
				direction = up;	// and direction to up to restart the process until...
			}
			else if (total_passengers == 0) // the total passengers equal zero
			{
				direction = down;	// then set the direciton to down
				state = moving;	// set state to moving
			}
		}
		break;
		}
	} while (occurrence < 50);
}

int Elevator::ReturnPassengersFloors()
{
		for ( int i = 0; i < 5; i++)
			       {
				cout << "Passengers on floor" << i+1 << " " << passenger_floor[i] << endl;
				Passengers_on_all_floors += passenger_floor[i];
				
					}
			cout << "Displaying all passengers in building\n" << Passengers_on_all_floors << endl;
	return Passengers_on_all_floors;
}

class Report 
{
public:
	void PrintReport(Elevator myarray);
	void SummaryReport();
};

void ReportHelper(int *elevator_id, int total_passengers, int total_passengers_on_floor, int total_time, 
	int floor_number, int pass_per_floor)
{
	for (int i = 0; i < 5; i++)
	{
		//cout << pass[i];			// print out the passengers on each floor
		//pass[elevator_id];
	}
}
void Report::PrintReport(Elevator myarray)
{
	
}

class PoissonDistribution
{
public:
	int interarrival_time;
	void set_IntervalTime();
	float get_Interval();
	float get_next(float mean);
};

void PoissonDistribution::set_IntervalTime()
{
	srand(unsigned (time(NULL) ) );
}

float PoissonDistribution::get_next(float mean)
{
	RAND_MAX;
	float R;
            float sum = 0;
            interarrival_time =-1;
            float z;
            while(sum <=mean)
            {
           R = (float)rand()/(float)(RAND_MAX+1);
           z = -log(R);
             sum+= z;
		    interarrival_time++;
            }
            return (float)interarrival_time;
}

float PoissonDistribution::get_Interval()
{
	return (float)interarrival_time;
}