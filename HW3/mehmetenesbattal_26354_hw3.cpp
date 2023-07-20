#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h> 
#include <queue> 
#include <semaphore.h>
using namespace std;

#define NUM_THREADS 10
#define MEMORY_SIZE 150

struct node
{
	int id;
	int size;
};


queue<node> myqueue; // shared que
pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_t server; // server thread handle
sem_t semlist[NUM_THREADS]; // thread semaphores

int thread_message[NUM_THREADS]; // thread memory information
int  memory[MEMORY_SIZE]; // memory size

bool allFinished = false; // indicates if all threads finshed, initially false
int nextAvailableIndex = 0; // indicates the next available index in the memory, initially zero


void my_malloc(int thread_id, int size)
{
	//This function will add the struct to the queue
	node *request = new node();
	request->id = thread_id;
	request->size = size;
	
	myqueue.push(*request);
}

void * server_function(void *)
{
	//This function should grant or decline a thread depending on memory size.
	while(!allFinished)
	{
		// continue to loop until all threads that use thread_function finishes
		while(!myqueue.empty())
		{
			// while request queue is not empty do the following
			node *request = new node();
			request = &myqueue.front();
			myqueue.pop();
			
			// if requested size is bigger than the available memory
			// sen -1 as thread message which indicates that there is nor enough memory
			if(MEMORY_SIZE - nextAvailableIndex < request->size)
				thread_message[request->id] = -1;
				
			// if there is enough memory for the request send the index of available memory as thread message
			else
				thread_message[request->id] = nextAvailableIndex;
			
			// increment the semaphore of the requesting thread so that it can continue
			sem_post(&semlist[request->id]);
		}
	}
}

void * thread_function(void * id) 
{
	//This function will create a random size, and call my_malloc
	//Block
	//Then fill the memory with id's or give an error prompt
	pthread_mutex_lock(&sharedLock); // lock the mutex
	
	int *idPointer = (int *) id;
	int size = (rand()% (MEMORY_SIZE/6)) +1; // get random size for the request
	
	my_malloc(*idPointer, size);
	
	// decrease semaphore of this thread and wait for server, will continue when server increments the semaphore
	sem_wait(&semlist[*idPointer]);
	
	// if not enough memory
	if(thread_message[*idPointer] == -1)
		cout << "Thread " << *idPointer << ": There is no available memory" << endl;
		
	else
	{
		// write the id of the thread to memory size times
		for(int i = 0; i < size; i++)	
			memory[nextAvailableIndex + i] = *idPointer;			
		
		// update the next available index of the memory
		nextAvailableIndex += size;
	}
	
	pthread_mutex_unlock(&sharedLock); // unlock the mutex
}

void init()	 
{
	pthread_mutex_lock(&sharedLock);	//lock
	for(int i = 0; i < NUM_THREADS; i++) //initialize semaphores
	{sem_init(&semlist[i],0,0);}
	for (int i = 0; i < MEMORY_SIZE; i++)	//initialize memory 
  	{memory[i] = 0;}
   	pthread_create(&server,NULL,server_function,NULL); //start server 
	pthread_mutex_unlock(&sharedLock); //unlock
}



void dump_memory() 
{
	// You need to print the whole memory array here.
	cout << "Memory :\n"; 
	for(int i = 0; i < MEMORY_SIZE; i++)
	{
		cout << memory[i];
	}
}

int main (int argc, char *argv[])
 {
 	//You need to create a thread ID array here
	int threadID [NUM_THREADS];
	
	for( int i = 0; i < NUM_THREADS; i++)
	{
		// thread ids are from 0 to (number of threads -1)
		threadID[i] = i;
	}

 	init();	// call init

 	//You need to create threads with using thread ID array, using pthread_create()
	pthread_t threads[NUM_THREADS];
	
	for(int i = 0; i < NUM_THREADS; i++)
	{
		pthread_create(&threads[i], NULL, thread_function, (void *) &threadID[i]);
	}

 	//You need to join the threads
	for(int i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
	// here all the threads joined, update allFinished
	allFinished = true;

 	dump_memory(); // this will print out the memory
 	
 	printf("\nMemory Indexes:\n" );
 	for (int i = 0; i < NUM_THREADS; i++)
 	{
 		printf("[%d]" ,thread_message[i]); // this will print out the memory indexes
 	}
 	printf("\nTerminating...\n");
	
	return 0;
 }