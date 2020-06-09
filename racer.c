// racer.c - a thread controlling a figure that races across a terminal window
// 
// @author rdb5063@rit.edu
// @date 11/12/18
// 
// This is the implementation for car racing threads interface

#define _DEFAULT_SOURCE

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#include "racer.h"

int FINISH_LINE;
int FLAT_TIRE_THRESHOLD = 3; //in seconds
long MAX_MILLIS_DELAY;
long MIN_MILLIS_DELAY = 0;

pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;

/// init_racers does setup work for all racers before the start of the race.
///
/// @param milliseconds the length of pause between steps in animation 
/// @param length  total length of the race determined by the terminal width
///
/// This must define the FINISH_LINE as (length - MAX_CAR_LEN - 2) because
/// the left end of the racer graphic has to cross the line and 
/// the value 2 is the margin of the racetrack.

void init_racers( long milliseconds, int length ){
	FINISH_LINE = length - MAX_CAR_LEN - 2;
	if(milliseconds == -1){
		MAX_MILLIS_DELAY = DEFAULT_WAIT;
	}
	else{
		MAX_MILLIS_DELAY = milliseconds;
	}
}

/// make_racer - Create a new racer.
///
/// @param name the string name to show on the display for this racer
/// @param row the row in which to race in the terminal
/// @return Racer pointer a dynamically allocated Racer object
/// @pre strlen( name ) <= MAX_NAME_LEN, for display reasons.

Racer * make_racer( char *name, int row ){
	Racer* myRacer = (Racer*) malloc(sizeof(Racer));
	myRacer->row = row;
	myRacer->distance = 0;

	//Create a generic vehicle graphic
	char* fullGraphic = (char*) malloc(sizeof(char)*13);
	fullGraphic[0] = '~';
	fullGraphic[1] = 'O';
	fullGraphic[2] = '=';
	fullGraphic[3] = '-';
	fullGraphic[4] = '-';
	fullGraphic[5] = '-';
	fullGraphic[6] = '-';
	fullGraphic[7] = '-';
	fullGraphic[8] = '-';
	fullGraphic[9] = '-';
	fullGraphic[10] = 'o';
	fullGraphic[11] = '>';

	//Overrite the generic vehicle with the specific name.
	for(int i=0;(unsigned int)i<strlen(name);i++){
		if(name[i] != '\0'){
			fullGraphic[i+3] = name[i];
		}
	}

	myRacer->graphic = fullGraphic;
	return myRacer;
}

/// destroy_racer - Destroy all dynamically allocated storage for a racer.
///
/// @param racer the object to be de-allocated

void destroy_racer( Racer *racer ){
	free(racer->graphic);
	free(racer);
}

/// This function is the 'main method' for a thread instance.
///
/// run function runs one racer in a race. here are the steps run takes:
///
/// Initialize the display of the racer:
///     Display the racer's graphic (a string) at its lane in the terminal,
///     and show the racer in its lane, with the racer's rear end in column 0.
///
/// Repeat actions below until the racer's distance equals the FINISH_LINE:
///
///     Calculate a random sleep value between 0 and the delay value 
///         given to init_racers at the start of the race.
///     If sleep value is less than or equal to 3, the racer gets a flat and
///         can't finish the race. Display an 'X' in the second character of
///         the graphic to show a flat tire.
///         Display the car with the flat tire and the function ends.
///     Otherwise, sleep for that length of time to indicate speed/slowness.
///
///     Change the display of this racer by moving 1 column to the right.
///         Erase the racer graphic from the display.
///         Increase the racer's distance by 1.
///         Re-display the racer's graphic at the new location.
///         Refresh the terminal.
///
///     Note: run must keep updates of the display by one racer "atomic".
///     That means when one racer is advancing on the screen, no other
///     racers can change the state of the terminal screen.
///
/// @param racer Racer object declared as void* for pthread operability
/// @return void pointer to status. A NULL represents success.
/// @pre racer cannot be NULL.

void* run( void *racer ){
	//draw racer with rear at column 0;
	Racer* myRacer;
	myRacer = (Racer*) racer;

	while(myRacer->distance <= FINISH_LINE){

		//lock screen
		pthread_mutex_lock(&mymutex);

		//move to line of racer
		move(myRacer->row,0);
		refresh();

		//clear line
		clrtoeol();

		//increment distance
		myRacer->distance = myRacer->distance + 1;

		//write to screen
		mvprintw(myRacer->row,myRacer->distance,myRacer->graphic);

		//refresh screen
		refresh();

		//unlock
		pthread_mutex_unlock(&mymutex);

		int speedDelay = abs(rand() % (MAX_MILLIS_DELAY + 1 - MIN_MILLIS_DELAY) + MIN_MILLIS_DELAY);
		if(speedDelay < FLAT_TIRE_THRESHOLD){

			//lock screen
			pthread_mutex_lock(&mymutex);

			//move to line of racer
			move(myRacer->row,0);
			refresh();

			//change to flat tire
			myRacer->graphic[1] = 'X';
			mvprintw(myRacer->row,myRacer->distance,myRacer->graphic);

			//clear line
			clrtoeol();

			//unlock
			pthread_mutex_unlock(&mymutex);

			//terminate thread
			pthread_exit(NULL);
			return (void*) 1;
		}
		else{
			//Convert the sleepDelay to microseconds and let the thread sleep.
			usleep(abs(rand()) % (speedDelay*1000));
		}
	}

	pthread_exit(NULL);
	return (void*) 0;
}
