#include "FitBitData.h"

/*******************************************************************************
 * Programmer: Drew Evensen		                                               *
 * Class: CptS 122; Lab Section 7                                              *
 * Programming Assignment: 1	                                               *
 * Date: 8/29/22                                                               *
 *                                                                             *
 * Description: This program reads from a csv file and converts it to          *
 *              datapoints, giving averages and totals of the users vitals     *
 ******************************************************************************/

// Checks a given entry point to a FitbitData entry for missing information and cleans it; z meaning invalid data
// Takes a string of data
// Returns a formatted and cleaned string
char* cleanData(char* dataEntry)
{
	// Buffer for the cleaned data
	static char cleanedLine[100] = "";
	int cleanI = 0;

	// While we have not cleaned the entire dataEntry:
	while (*dataEntry != '\0')
	{
		// Check for missing data in the middle of the entry
		if (dataEntry[0] == ',' && dataEntry[1] == ',')
		{
			// Fill missing data
			cleanedLine[cleanI++] = dataEntry[0];
			cleanedLine[cleanI++] = 'z';
			dataEntry++;

			// Check to see if there are multiple spaces missing in a row
			continue;
		}

		// Check for missing data at the end of the entry
		if (dataEntry[0] == ',' && dataEntry[1] == '\n')
		{
			// Fill missing data
			cleanedLine[cleanI++] = dataEntry[0];
			cleanedLine[cleanI++] = 'z';

			// Do not add an extra comma
			break;
		}

		// Copy data to the cleaned line
		cleanedLine[cleanI++] = dataEntry[0];
		dataEntry++;
	}

	// Add the null character
	cleanedLine[cleanI] = '\0';

	return cleanedLine;
}

// Adds a FitbitData entry to the FitbitData array at a given index
// Takes a FitbitData point, FitbitData array, and an integer; assumes integer is within range of the array
void copyFBData(FitbitData* dataPoint, FitbitData* dataArray, int index)
{
	strcpy(dataArray[index].patient, dataPoint->patient);
	strcpy(dataArray[index].minute, dataPoint->minute);

	dataArray[index].calories = dataPoint->calories;
	dataArray[index].distance = dataPoint->distance;

	dataArray[index].floors = dataPoint->floors;
	dataArray[index].heartRate = dataPoint->heartRate;
	dataArray[index].steps = dataPoint->steps;

	dataArray[index].sleepLevel = dataPoint->sleepLevel;
}

// Converts a given line of data to a FitbitData point and adds it to an array, excluding duplicates
// Takes a char pointer and a FitbitData array; assumes the char pointer is already formatted
void dedupeArrayInsert(char* line, FitbitData* dataArray)
{
	// Traverses the FitbitData array
	int index = 0;
	// Tracks possibility of a duplicate; 0 = false, 1 = true
	int dupe = 0;

	// Carries data to be inserted
	char data[30];

	// Create a new FitbitData point and assign the correct ID
	FitbitData newData;
	strcpy(newData.patient, line);

	// Move through the line, filling in data
	// Extract the time
	strcpy(data, strtok(NULL, ","));
	if (strcmp(data, "z") == 0)
		strcpy(data, "N/A");
	strcpy(newData.minute, data);

	// Extract double values
	for (int i = 0; i < 2; i++)
	{
		strcpy(data, strtok(NULL, ","));
		if (strcmp(data, "z") == 0)
			strcpy(data, " -1.0");

		if (i < 1)
			newData.calories = atof(data);
		else
			newData.distance = atof(data);
	}

	// Extract unsigned integer values
	for (int i = 0; i < 3; i++)
	{
		strcpy(data, strtok(NULL, ","));
		if (strcmp(data, "z") == 0)
			strcpy(data, "49494949");

		if (i == 0)
			newData.floors = atoi(data);
		else if (i == 1)
			newData.heartRate = atoi(data);
		else
			newData.steps = atoi(data);
	}

	// Extract sleep value
	strcpy(data, strtok(NULL, ","));
	if (strcmp(data, "z") == 0)
		strcpy(data, "-1");
	newData.sleepLevel = atoi(data);

	// Check for duplicates
	while (strcmp(dataArray[index].patient, "") != 0)
	{
		if (strcmp(dataArray[index].minute, newData.minute) == 0)
		{
			dupe = 1;
			break;
		}
		index++;
	}

	// Add to the array if there are no duplicates present
	if (dupe == 0)
		copyFBData(&newData, dataArray, index);
}

// Goes through a csv file of FitbitData for a specific ID before adding to a FitbitData array.
// Takes a FILE pointer and a FitbitData array.
void fileScan(FILE* fileLoc, FitbitData* dataArray)
{
	// Catches both the line of data and desired ID
	char line[100] = "";
	char target[6] = "";

	// Find the target ID
	fgets(line, 100, fileLoc);
	strcpy(target, strtok(strchr(line, ','), ",")); // Copy the token between the first and second ',' in line to target (second cell)
	fgets(line, 100, fileLoc);

	// Extract and clean each line of the file
	while (fgets(line, 100, fileLoc) != NULL)
	{
		strcpy(line, cleanData(line));

		// Filter the lines for the target ID
		if (strcmp(target, strtok(line, ",")) == 0)
			dedupeArrayInsert(line, dataArray);
	}
}

// Sends a FitbitData point to a file and echoes what is sent to the console
// Takes a File location to be sent to and a FitbitData point
void fitbitDataToString(FILE* fileLoc, FitbitData dataPoint)
{
	// Print to file
	fprintf(fileLoc, "%s,", dataPoint.patient);
	fprintf(fileLoc, "%s,", dataPoint.minute);
	fprintf(fileLoc, "%f,", dataPoint.calories);
	fprintf(fileLoc, "%f,", dataPoint.distance);
	fprintf(fileLoc, "%i,", dataPoint.floors);
	fprintf(fileLoc, "%i,", dataPoint.heartRate);
	fprintf(fileLoc, "%i,", dataPoint.steps);
	fprintf(fileLoc, "%i\n", dataPoint.sleepLevel);

	// Print to console
	printf("%s,", dataPoint.patient);
	printf("%s,", dataPoint.minute);
	printf("%f,", dataPoint.calories);
	printf("%f,", dataPoint.distance);
	printf("%i,", dataPoint.floors);
	printf("%i,", dataPoint.heartRate);
	printf("%i,", dataPoint.steps);
	printf("%i\n", dataPoint.sleepLevel);
}