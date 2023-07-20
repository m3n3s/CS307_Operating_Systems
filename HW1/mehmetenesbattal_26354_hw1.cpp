#include <stdlib.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;

// Mehmet Enes Battal - 26354 - CS307 HW1

// global variables
vector< vector<int> > seats(2, vector<int>(50, 0));
int capacity = 100;
int turn = 1;

void* travelAgency(void* idP)
{
	int* idPointer = (int*)idP;
	int id = *idPointer;
	while(true)
	{
		// random number between 1 and 100
		int seatNum = rand()%100 + 1;
		
		while(turn != id){};
		
		// get in critical region
		if(capacity != 0) //flight is not full
		{
			cout << "Agency " << id << " Entered Critical Region" << endl;

			// reserve a seat, update capacity
			// floor division for row, modulus for column
			if(seats[(seatNum-1)/50][(seatNum-1)%50] == 0)
			{
				seats[(seatNum-1)/50][(seatNum-1)%50] = id;
				capacity--;
				cout << "Seat Number " << seatNum << " is reserved by Agency " << id << endl;			
			}			
			
			cout << "Agency " << id << " Exit Critical Region" << endl << endl;

			// get out of critical region
			if(id == 1)
				turn = 2;
				
			else if(id == 2)
				turn = 1;
		}
		
		else // capacity is full, return
			return NULL;
	}
}

int main()
{	
	srand(time(NULL));
	
	int id1 = 1, id2 = 2;
	pthread_t agency1, agency2;
	
	pthread_create(&agency1, NULL, travelAgency, (void*) &id1);
	pthread_create(&agency2, NULL, travelAgency, (void*) &id2);
	
	// constantly check capacity
	while(true)
	{
		if(capacity == 0)
		{			
			// print matrix to console
			cout << "No Seats Left" << endl << endl << endl;
			cout << "Plane is full: " << endl << endl;
			
			for(int i = 0; i < seats.size(); i++)
			{
				for(int k = 0; k < seats[0].size(); k++)
					cout << seats[i][k] << " ";
					
				cout << endl;
			}
			
			return 0;
		}
	}
	
	return 0;
}