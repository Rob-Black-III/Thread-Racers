//
// File: pt-cruisers.c
// pt-cruisers.c is a "game" that shows how fast threads execute.
// @author Rob Black rdb5063@rit.edu
// // // // // // // // // // // // // // // // // // // // // // //

#define _DEFAULT_SOURCE
#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <pthread.h>
#include <string.h>

#include "myUtils.h"
#include "racer.h"

//Global Variables for ncurses and delay speed.
WINDOW *window;
int MAX_SPEED_DELAY;

/**
 * main is the main function of the game. It takes command-line arguments
 * specifying a delay speed and/or names for the thread 'racers'.
 * @param int argc - number of arguments passed to the programs
 * @param char** argv - double pointer char array to arguments.
 * @return int EXIT_STATUS - exit code of the program
 */
int main(int argc, char** argv){

	int numThreads = 0;
	Racer** allRacers = NULL;
	for(int i=1;i<argc;i++){
		if(i == 1){
			if(isdigit(argv[1][0]) != 0 || argv[1][0] == '-'){
				int number = atoi(argv[1]);
				if(number <= 0){
					fprintf(stderr,"Error: delay (%d) is invalid.\n", number);
					MAX_SPEED_DELAY = -1;
					return EXIT_FAILURE;
				}
				else{
					printf("MAX_SPEED_DELAY: %d\n",number);
					MAX_SPEED_DELAY = number;
				}
				continue;
			}
			else{
				MAX_SPEED_DELAY = -1;
			}
		}
		else if(strlen(argv[i]) > MAX_NAME_LEN){
			fprintf(stderr,"Error: racer %s exceeds length limit of %d.\n",argv[i],MAX_NAME_LEN);
			return EXIT_FAILURE;
		}
		numThreads++;
		allRacers = (Racer**) realloc(allRacers,(numThreads * sizeof(Racer)));
		allRacers[numThreads-1] = make_racer(argv[i],numThreads); //numThreads is also the rows.
	}

	//Checks if the user provided atleast two names for the racers.
	if(numThreads <= 2){
		//print Usage
		fprintf(stderr,"Usage: pt-crusiers [max-speed-delay] name1 name2 [name3...]\n");
		return EXIT_FAILURE;
	}


	//Ncurses initialization code
	window = initscr();
	cbreak();
	noecho();
	int width = getmaxx(stdscr);
	
	//SEED for Random Generator
	srand(time(NULL));

	//thread array
	pthread_t threads[numThreads];
	void* retval;

	//initialize variables for the race
	init_racers(MAX_SPEED_DELAY, width);	

	//spawn threads	
	for(int t=0;t<numThreads;t++){
		int code = pthread_create(&threads[t], NULL, run, (void*) allRacers[t]);

		if(code){
			printf("ERROR; pthread_create() returned %d\n", code);
			exit(-1);
		}
	}

	//wait for threads to terminate
	for(int t=0;t<numThreads;t++){
		pthread_join(threads[t],&retval);
	}

	/*
	//Free all Racer's created;
	for(int i=numThreads-1;i>0;i++){
		//destroy_racer(allRacers[i]);
	}

	free(allRacers);
	*/

	//Exit on keypress loop.
	while(1){
		mvprintw(0, 0, "The race is over! Press any key to exit the racetrack.");
		getch();
		endwin();
		break;
	}
	return 0;
}
