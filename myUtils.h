// myUtils.h - A collection of useful functions I made for all C projects.
//
// @author rdb5063@rit.edu
// @date 11/12/18
//
// This is my collection of useful functions.

#ifndef _MYUTILS_H
#define _MYUTILS_H

#define TRUE 1
#define FALSE 0

#endif


/**
 * Creates a two-dimensional dynamically allocated array.
 * @param int rows - rows of the array
 * @param int cols - cols of the array
 * @return int** array - double-pointer to the 
 * first index of an int array
 */
int** makeTwoDimensionalIntArray(int rows, int cols);

/**
 * Frees a two-dimensional dynamically allocated array.
 * @param int** array - double-pointer to the 
 * first index of an int array
 * @param int rows - rows of the array
 */
void freeTwoDimensionalIntArray(int** array, int rows);


/**
 * Checks to see if a string can also be represented as a digit.
 * Uses int isdigit(char ch) as a helping function.
 * @param char* charArray - an array of characters
 * @param int rows - rows of the array
 * @return int - 1 if a digit, 0 if not a digit
 */
int isDigit(char* charArray);

/**
 * Returns the absolute value of an integer.
 * @param int number - a positive or negative integer.
 * @return int number - a positve variation of the number
 * provided via the argument.
 */
int abs(int number);
