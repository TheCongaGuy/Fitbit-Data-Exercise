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

int main(int argc, char argv[])
{
	// Averages and Totals
	double tCal = 0, tDst = 0;
	unsigned int tFlr = 0, tStp = 0, aHrt = 0, tHrt = 0, tMinStp = 0;

	// Worst sleep
	char slStartT[9] = "", slEndT[9] = "", slPotenT[9] = "";
	int qSleep = 0, wSleep = 0;

	// Array to store 24 hours of data from the fitbit
	FitbitData dailyActive[1440] = {"", "", 0, 0, 0, 0, 0, 0};

	// File pointers
	FILE* inFitbitData = fopen("FitbitData.csv", "r");
	FILE* outFitbitData = fopen("FitbitResults.csv", "w");

	// Check to see if files were opened successfully
	if (inFitbitData == NULL)
	{
		// If infile was not opened, display error message and close code -1
		printf("ERROR OPENING INPUT FILE");
		return -1;
	}
	if (outFitbitData == NULL)
	{
		// If outfile was not opened, display error message and close code -2
		printf("ERROR OPENING OUTPUT FILE");
		return -2;
	}

	// Extract data
	fileScan(inFitbitData, dailyActive);

	// Close the read file
	fclose(inFitbitData);

	// Find totals/averages for each field
	for (int i = 0; i < 1440; i++)
	{
		// Skip over empty FitbitData points
		if (strcmp(dailyActive[i].minute, "") == 0)
			continue;

		// Ignore invalid data in floating point values
		if (dailyActive[i].calories != -1)
			tCal += dailyActive[i].calories;
		if (dailyActive[i].distance != -1)
			tDst += dailyActive[i].distance;

		// Ignore invalid data in unsigned int values
		if (dailyActive[i].floors != 49494949)
			tFlr += dailyActive[i].floors;
		if (dailyActive[i].steps != 49494949)
		{
			tStp += dailyActive[i].steps;
			// Record max steps in a minute
			if (dailyActive[i].steps >= tMinStp)
				tMinStp = dailyActive[i].steps;
		}

		// Skip invalid heart rate values
		if (dailyActive[i].heartRate != 49494949)
		{
			aHrt += dailyActive[i].heartRate;
			tHrt++;
		}

		// Find the begining of worst quality sleep
		if (dailyActive[i].sleepLevel > 1)
		{
			if (qSleep == 0)
				strcpy(slPotenT, dailyActive[i].minute);

			qSleep += dailyActive[i].sleepLevel;
		}
		else // Update worst quality sleep with the potential quality sleep if potential is worse than current worst
		{
			if (qSleep > wSleep)
			{
				strcpy(slEndT, dailyActive[i - 1].minute);
				strcpy(slStartT, slPotenT);
			}

			wSleep = qSleep;
			qSleep = 0;
		}
	}
	// Calculate heart rate average
	aHrt /= tHrt;

	// Format output file
	fprintf(outFitbitData, "\"Total Calories\",\"Total Distance\",\"Total Floors\",\"Total Steps\",\"Avg Heartrate\",\"Max Steps\",Sleep\n");
	fprintf(outFitbitData, "%f,%f,%d,%d,%d,%d,%s,%s\n", tCal, tDst, tFlr, tStp, aHrt, tMinStp, slStartT, slEndT);

	// Echo to console
	printf("Total Calories, Total Distance, Total Floors, Total Steps, Avg Heartrate, Max Steps, Sleep\n");
	printf("%f,\t%f,\t%d,\t\t%d,\t\t%d,\t\t%d,\t%s,\t%s\n", tCal, tDst, tFlr, tStp, aHrt, tMinStp, slStartT, slEndT);

	// Convert each data entry in the 24 hour array to a string
	for (int i = 0; i < 1440; i++)
		fitbitDataToString(outFitbitData, dailyActive[i]);

	// Close the write file
	fclose(outFitbitData);

	return 0;
}