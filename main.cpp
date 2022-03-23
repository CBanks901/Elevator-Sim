/*
PURPOSE: This program illustrates how to create an eventlist class and print out to a 
		 CSV( Comma Separated Values )file which can be opened by EXCEL
COURSE EXAMPLE: GSP281 
*/
#include "Elevatordata.h"
#include<iostream>
using std::cout;
using std::cin;
using std::endl;

#include <iomanip>
using namespace std;
#include <conio.h>

#include <fstream>

/***************************************************
 * an example Event where all times are in seconds *
 ***************************************************/
struct Event
{
 int eventNumber;		//could also double as the passenger number
 int interArrivalTime;  //incremental time to next arrival
 int arrivalTime;		//when passenger first arrived on the floor
 int startingFloor;     //passenger's starting floor
 int destinationFloor;  //passenger's destination floor
 int serviceStartTime;  //when elevator picked up passenger
 int serviceEndTime;    //time passenger is delivered to their floor

};

/**************************************************************
 * GLOBAL VARAIBLES DEFINING EVENT LIST SIZE AND CSV FILENAME *
 **************************************************************/
char fileName[] = "EventList.csv";    //filename to output CSV(Comma Separated Values) file
const int SIZE = 750; //sets the number of events to be placed in the eventlist


/***********************
 * AN EVENT LIST CLASS *
 ***********************/

//A partially completed EventList class
class EventList
{
private:
 Event eventList[SIZE];

public:
EventList();  //simple constructor which zeroes out all event list data
void updateEventInfo(Event& update); //set an event in the eventList information
void getEventInfo(int eventnumber, Event& event); //get a particular event
void printEventListToScreen(int n); //print the first 'n' events to the screen
void printEventListToCSVFile(int n); //print the first 'n' events to a file

void First400Arrivals(int startTime, int endTime, int numberOfPeople); //Linear distribution of interarrival times and 
																		//destination floor
};

EventList::EventList()
{
 //initialize array of size SIZE events
	for(int i=0; i<SIZE; i++)
	{
		 eventList[i].eventNumber = i + 1;
		 eventList[i].interArrivalTime = 0;
		 eventList[i].arrivalTime = 0;
		 eventList[i].startingFloor = 0;
		 eventList[i].destinationFloor = 0;
		 eventList[i].serviceStartTime = 0;
		 eventList[i].serviceEndTime = 0;
	}
}


void EventList::updateEventInfo(Event& update)
{
  eventList[update.eventNumber - 1].interArrivalTime = update.interArrivalTime;
  eventList[update.eventNumber - 1].arrivalTime = update.arrivalTime;
  eventList[update.eventNumber - 1].startingFloor = update.startingFloor;
  eventList[update.eventNumber - 1].destinationFloor = update.destinationFloor;
  eventList[update.eventNumber - 1].serviceStartTime = update.serviceStartTime;
  eventList[update.eventNumber - 1].serviceEndTime = update.serviceEndTime;
}

void EventList::getEventInfo(int eventnumber, Event& event)
{
 event.eventNumber = eventList[eventnumber].eventNumber;
 event.interArrivalTime = eventList[eventnumber].interArrivalTime;
 event.arrivalTime = eventList[eventnumber].arrivalTime;
 event.startingFloor = eventList[eventnumber].startingFloor;
 event.destinationFloor = eventList[eventnumber].destinationFloor;
 event.serviceStartTime = eventList[eventnumber].serviceStartTime;
 event.serviceEndTime = eventList[eventnumber].serviceEndTime;
}


void EventList::printEventListToScreen(int n)
{
 cout<<"Event    inter       arrival  starting   destination  service  service"<<endl
	<<"Number   ArrivalTime   Time    Floor        Floor       Start     End"<<endl;
    for(int i=0; i<n; i++)
	  {
		cout<<setw(4)<<eventList[i].eventNumber
			<<setw(11)<<eventList[i].interArrivalTime
			<<setw(11)<<eventList[i].arrivalTime
			<<setw(10)<<eventList[i].startingFloor
			<<setw(10)<<eventList[i].destinationFloor
			<<setw(12)<<eventList[i].serviceStartTime
			<<setw(10)<<eventList[i].serviceEndTime
			<<endl;
	  }
}

void EventList::printEventListToCSVFile(int n)
{
//open file and write over its' contents
ofstream outputfile;

outputfile.open(fileName, ios::out); //add the ios::app argument to append data

 if ( outputfile.is_open() )
  {
   outputfile<<"eventNumber,interArrival,arrivalTime,startingFloor,destinationFloor,serviceStart,serviceEnd"<<endl;

    for(int i=0; i<n; i++)
	  {
		outputfile<<eventList[i].eventNumber
			<<","<<eventList[i].interArrivalTime
			<<","<<eventList[i].arrivalTime
			<<","<<eventList[i].startingFloor
			<<","<<eventList[i].destinationFloor
			<<","<<eventList[i].serviceStartTime
			<<","<<eventList[i].serviceEndTime
			<<endl;
	  }

     outputfile.close();
	
  }
 
 else
   cout<<"ERROR: Unable to open the file"<<endl;
}

//create linearly spaced inter arrival times in the event list
void EventList::First400Arrivals(int startTime, int endTime, int numberOfPeople)
{
  float linearInterArrivalTime = (float(endTime) - startTime)/ numberOfPeople ; //time inerval in seconds
  float previousArrival = 0;
  float nextArrival = linearInterArrivalTime + previousArrival;
  Event tempEvent;

  tempEvent.startingFloor = 1;
  tempEvent.serviceStartTime = tempEvent.serviceEndTime = 0;
  
  int previousFloor = 1;

	for ( int i= 1; i <= numberOfPeople; i++ )
	{
	   tempEvent.eventNumber = i;
	   tempEvent.interArrivalTime = int (linearInterArrivalTime);
	   tempEvent.arrivalTime = int (nextArrival);
	   nextArrival +=linearInterArrivalTime;
	      previousFloor++;
		    if (previousFloor > 5)
				previousFloor = 2;
	   tempEvent.destinationFloor = previousFloor;
		updateEventInfo(tempEvent);

	}
}


/************************************************************
 * TEST THE EVENTLIST CLASS METHODS FOR BASIC FUNCTIONALITY *
 ************************************************************/
int main()
{
	cout << "Please enter a number 1 through 8 to specify how many elevators you want to have in the building.\n";
	int el;
	cin >> el;
	Elevator simu(el);
	Passenger *passn;
	Passenger pass[400];
	passn = pass;
	simu.RunElevator(passn);
	simu.ReturnPassengersFloors();

//create an eventList object
EventList myEvents;

cout<<"THE FIRST 5 EVENTS IN THE NEWLY CREATED EVENT LIST"<<endl<<endl;
myEvents.printEventListToScreen(400);  //print the first 5 entries to the screen

myEvents.printEventListToCSVFile(400); //print the first 10 entries to a file

//create some sample data for EVENT 4
Event sampleEvent[400];  
for (int i = 0; i < 400; i++){
sampleEvent[i].eventNumber = 4;
sampleEvent[i].interArrivalTime = 1;
sampleEvent[i].arrivalTime = 1;
sampleEvent[i].startingFloor = 1;
sampleEvent[i].destinationFloor = 4;
sampleEvent[i].serviceStartTime = 3;
sampleEvent[i].serviceEndTime = 39;

myEvents.updateEventInfo(sampleEvent[i]);
myEvents.getEventInfo(i, sampleEvent[i]);
}
cout<<"\n\n\nTHE FIRST 5 EVENTS AFTER UPDATING EVENT 4"<<endl<<endl;
myEvents.printEventListToScreen(400);  //print the first 5 entries to the screen
myEvents.First400Arrivals(0, 1800, 400);
myEvents.printEventListToScreen(401);
	
		// returns each passenger for every floor
	/*PoissonDistribution distribution;

	simu.RunElevator(passn);
	distribution.set_IntervalTime();*/
EventList lunch;
Event lunchEvent1[200];
for (int i = 0; i < 200; i++){
	//distribution.set_IntervalTime();
lunchEvent1[i].eventNumber = i;
lunchEvent1[i].interArrivalTime = 1;
lunchEvent1[i].arrivalTime = 2;
lunchEvent1[i].startingFloor = passn->destination();
lunchEvent1[i].destinationFloor = 1;	// destination floor has to be one
lunchEvent1[i].serviceStartTime = pass[i].loadtime() * i;
lunchEvent1[i].serviceEndTime = i/pass[i].loadtime();
}


for (int i = 0; i < 200; i++)
{
lunch.updateEventInfo(lunchEvent1[i]);
lunch.getEventInfo(i, lunchEvent1[i]);
}
lunch.printEventListToCSVFile(200);



cout << "\n\n WORKERS NOW GOING TO LUNCH\n" << endl << endl;

//lunch.printEventListToScreen(5);
lunch.First400Arrivals(16200, 17100, 200);	// 12;00 to 12:15
lunch.printEventListToScreen(201);
EventList lunch2L;
Event lunch2[200];

for (int i = 0; i < 200; i++)
{
		//distribution.set_IntervalTime();
lunch2[i].eventNumber = i;
lunch2[i].interArrivalTime = 1;
lunch2[i].arrivalTime = 2;
lunch2[i].startingFloor = passn->destination();
lunch2[i].destinationFloor = 1;	// destination floor has to be one
lunch2[i].serviceStartTime = pass[i].loadtime() * i;
lunch2[i].serviceEndTime = i/pass[i].loadtime();
}

for (int i = 0; i < 200; i++)
{
lunch2L.updateEventInfo(lunch2[i]);
lunch2L.getEventInfo(i, lunch2[i]);
}
lunch2L.printEventListToCSVFile(200);

cout << "\n\n OTHER WORKERS NOW GOING TO LUNCH\n" << endl << endl;

//lunch.printEventListToScreen(5);
lunch2L.First400Arrivals(17100, 18000, 200);	// 12;00 to 12:15
lunch2L.printEventListToScreen(201);

EventList lunchW;
Event lunchreturn[400];

for (int i = 0; i < 200; i++)
{
		//distribution.set_IntervalTime();
lunchreturn[i].eventNumber = i;
lunchreturn[i].interArrivalTime = 1;
lunchreturn[i].arrivalTime = 2*i;
lunchreturn[i].startingFloor = 1;
lunchreturn[i].destinationFloor = pass->destination();	// destination floor has to be one
lunchreturn[i].serviceStartTime = pass[i].loadtime() * i;
lunchreturn[i].serviceEndTime = i/pass[i].loadtime();
}

for (int i = 0; i < 200; i++)
{
lunchW.updateEventInfo(lunchreturn[i]);
lunchW.getEventInfo(i, lunchreturn[i]);
}
lunchW.printEventListToCSVFile(200);

cout << "\n\n OTHER WORKERS NOW GOING TO LUNCH\n" << endl << endl;

//lunch.printEventListToScreen(5);
lunchW.First400Arrivals(18900, 19800, 400);	// 12:4500 to 1:00
lunchW.printEventListToScreen(401);
EventList leaving;
Event leave[400];

for (int i = 0; i < 400; i++)
{
		//distribution.set_IntervalTime();
leave[i].eventNumber = i;
leave[i].interArrivalTime = 1;
leave[i].arrivalTime = 2;
leave[i].startingFloor = pass->destination();
leave[i].destinationFloor = 1;	// destination floor has to be one
leave[i].serviceStartTime = pass[i].loadtime() * i;
leave[i].serviceEndTime = leave[i].serviceStartTime/pass[i].loadtime();
}

for (int i = 0; i < 400; i++)
{
leaving.updateEventInfo(leave[i]);
leaving.getEventInfo(i, leave[i]);
}
leaving.printEventListToCSVFile(400);

cout << "\n\n OTHER WORKERS NOW GOING TO LUNCH\n" << endl << endl;

//lunch.printEventListToScreen(5);
leaving.First400Arrivals(18900, 19800, 400);	// 12:4500 to 1:00
leaving.printEventListToScreen(401);
// pause
      cout << "\nPress any key to continue...";
      cin.sync();
      _getch();

      // return environment variable
	return 0;
}