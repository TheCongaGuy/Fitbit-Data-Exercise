#ifndef FIT_BIT_DATA_H // Guard Code start
#define FIT_BIT_DATA_H

#define _CRT_SECURE_NO_WARNINGS

/*******************************************************************************
 * Programmer: Drew Evensen		                                               *
 * Class: CptS 122; Lab Section 7                                              *
 * Programming Assignment: 1	                                               *
 * Date: 8/29/22                                                               *
 *                                                                             *
 * Description: This program reads from a csv file and converts it to          *
 *              datapoints, giving averages and totals of the users vitals     *
 ******************************************************************************/

#include <stdio.h> // fprintf(), printf(), fopen(), fclose(), fgets()
#include <stdlib.h> // atoi(), atof()
#include <string.h> // strtok(), strcpy(), strchr(), strcmp()

// Enumerated data type to represent the wakefulness recorded in someone's sleep activity
typedef enum sleep
{
    NONE = 0, ASLEEP = 1, AWAKE = 2, REALLYAWAKE = 3
} Sleep;

// Struct Data type to represent the data recorded on a fitbit watch
typedef struct fitbit
{
    char patient[10];
    char minute[9];

    double calories;
    double distance;

    unsigned int floors;
    unsigned int heartRate;
    unsigned int steps;

    Sleep sleepLevel;
} FitbitData;

// Goes through a csv file of FitbitData points for a specific ID before adding to a FitbitData array.
// Takes the file location and the array to be added to
void fileScan(FILE* fileLoc, FitbitData* dataArray);

// Sends a FitbitData point to a file and echoes what is sent to the console
// Takes a File location to be sent to and a FitbitData point
void fitbitDataToString(FILE* fileLoc, FitbitData dataPoint);

#endif // Guard Code end