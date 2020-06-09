//
// File: myUtils.c
// myUtils.c is a utils file with functions I may use
// @author Rob Black rdb5063@rit.edu
// // // // // // // // // // // // // // // // // // // // // // //

#include <ctype.h>
#include <stdlib.h>

#include "myUtils.h"

/**
 * Creates a two-dimensional dynamically allocated array.
 * @param int rows - rows of the array
 * @param int cols - cols of the array
 * @return int** array - double-pointer to the 
 * first index of an int array
 */
int** createTwoDimensionalIntArray(int rows, int cols){
	int** array;
	array = malloc(rows * sizeof(int*));
	for(int i=0;i<rows;i++){
		array[i] = malloc(cols * sizeof(int));
	}
	return array;
}

/**
 * Frees a two-dimensional dynamically allocated array.
 * @param int** array - double-pointer to the 
 * first index of an int array
 * @param int rows - rows of the array
 */
void freeTwoDimensionalIntArray(int** array, int rows){
	for(int i = rows-1;i >= 0;i--){
		free(array[i]);
	}
	free(array);
}


/**
 * Checks to see if a string can also be represented as a digit.
 * Uses int isdigit(char ch) as a helping function.
 * @param char* charArray - an array of characters
 * @param int rows - rows of the array
 * @return int - 1 if a digit, 0 if not a digit
 */
int isDigit(char* charArray){
	unsigned int i = 0;
	
	if(charArray[0] == '-'){
		i = 1;
	}
	for(;i<(sizeof(charArray)/sizeof(char));i++){
		if(!isdigit(charArray[i])){
			return FALSE;
		}
	}
	return TRUE;
}

/**
 * Returns the absolute value of an integer.
 * @param int number - a positive or negative integer.
 * @return int number - a positve variation of the number
 * provided via the argument.
 */
int abs(int number){
	if(number >= 0){
		return number;
	}
	else{
		return number * -1;
	}
}
