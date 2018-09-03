#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>
#include <time.h>
#include "bus.h"

#define DATA_SIZE 10
#define MEMBERS 2
#define TRANSMISSIONS 5 
#define SLEEP_TIME	1

void * run(void * arg)
{
	struct legacy packet;
	struct legacy rcv;
		
	int id;
	char *msg = "active";
	int counter = 0;
	
	id = (void *)arg;
	
	packet.id = id;
	packet.flag = counter;
	strcpy(packet.message, msg);	

	if( id == 0) {
	
		broadcast(&packet);	
		counter++;
		packet.flag = counter;
		strcpy(packet.message, msg);	
		pthread_yield();
		sleep(SLEEP_TIME);
		
		while( counter < TRANSMISSIONS) {
		
			rcv = readBus();
			
			if(rcv.id == 1) {
				packet.id = id;
				packet.flag = rcv.flag + 1;
				broadcast(&packet);		
				counter++;	
				pthread_yield();
				sleep(SLEEP_TIME);
			}
		}
	}
	
	if( id == 1) {
			
		while(counter < TRANSMISSIONS) {	

			rcv = readBus();
		
			if(rcv.id == 0) {
				packet.id = id;
				packet.flag = rcv.flag + 1;
				broadcast(&packet);		
				counter++;	
				pthread_yield();
				sleep(SLEEP_TIME);
			}
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	initBus();
	printf("Bus initialized.\n");

	
	pthread_t node[MEMBERS];

	for(int i = 0; i < MEMBERS; i++)
		pthread_create( &node[i], NULL,(void *)run, (void *)i);

	for(int i = 0; i < MEMBERS; i++)
		pthread_join(node[i], NULL);	

	printf("All threads completed.\n");	
	
	return(0);
}
