/*
--------------------------------------------------------------   
			bus.h
--------------------------------------------------------------   
	Bus structure represents the bus, while defining 
	various functionalities and synchronization primitives
	for the correct workflow of the messages.
	One sender is accessing the bus at a time.
 */
#include <string.h>

#define BUS_WRITTEN	0x2
#define DATA_SIZE	10

struct legacy {
	unsigned int id;
	unsigned int flag;
	char message[DATA_SIZE];	
}BUS;

pthread_mutex_t bmtx = PTHREAD_MUTEX_INITIALIZER;

// initializing the Bus instance.
void initBus(void)
{
	BUS.id = 0x0;
	BUS.flag = 0x0;
	strcpy(BUS.message,"");
}

// displaying what is written on the Bus.
void printBus( struct legacy *bus)
{
	printf("Bus::  \n");	
	printf("ID: %3d\t", bus->id);
	printf("flag: %3d\t", bus->flag);
	printf("Message: %10s\n", bus->message);
}

// writing on the Bus.
int writeBus( struct legacy *p)
{


	pthread_mutex_lock(&bmtx);	

		BUS.id = p->id;
		BUS.flag = p->flag;
		strcpy(BUS.message, p->message);

		printBus(&BUS);
	pthread_mutex_unlock(&bmtx);
	return BUS_WRITTEN;

}

// function called by main program.
int broadcast(struct legacy *p)
{
	return writeBus(p);
}

struct legacy readBus(void) 
{
	struct legacy p;

	pthread_mutex_lock(&bmtx);

		p.id = BUS.id;
		p.flag = BUS.flag;
		strcpy(p.message, BUS.message);
	pthread_mutex_unlock(&bmtx);	

	return p;
}
