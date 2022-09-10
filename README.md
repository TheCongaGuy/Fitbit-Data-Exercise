# Fitbit-Data-Exercise
This was my first programming assignment not only at Washington State University, but also my first time programming in C. I have messed around in C++ before which I feel helped my understanding, but it was still quite a challenge. I have never delt with a file of this size before but I am mostly happy with my program. I do wish to improve on the chained if statements checking for invalid data. Overall however, for my very first program in C I am quite happy with how it turned out. What this program does is take a csv file of fitbit data and calculate average heart rate, max steps, total steps, etc. of the user in a 24 hour period.
# Program Operation
The start of this program requires a csv file of fitbit data points like so:
| patient     | minute      | calories    | distance    | floors      | heart       | steps       | sleep_level |
|    :----:   |    :----:   |    :----:   |    :----:   |    :----:   |    :----:   |    :----:   |    :----:   |
| 12cx7       | 0:00:00     | 0.0         | 0.0         | 0           | 70          | 0           | 0           |

This line is supposed to replicate the data tracked by a Fitbit watch, with the patient ID belonging to a specific user.
Patient and minute values are to be stored as a string while the rest are stored as doubles or integers. Sleep level however is recorded in an enumerated type to give meaning to the data. 0 = No sleep was recorded; 1 = Sleep was recorded; 2 = Sleep was recorded, but REM was not achieved; 3 = Sleep was interupted.

In this form it would be quite easy to parse the string using `strtok()` and using the index of the token to assign values. However, sometimes the fitbit would fail to read data resulting in a line like this:
| patient     | minute      | calories    | distance    | floors      | heart       | steps       | sleep_level |
|    :----:   |    :----:   |    :----:   |    :----:   |    :----:   |    :----:   |    :----:   |    :----:   |
| 12cx7       | 0:01:00     |             | 0.0           | 0           |             | 0           |             |

Trying to parse this kind of string in a loop is impossible. Either slots will be skipped and data will be sotred in incorrect fileds, or we will attempt to insert NULL data into an ingeger or double value, giving incorrect results. Best case scenario, our calculations at the end of this program will be really weird, though what happens most of the time is the program crashing entirely.

Another problem to look out for is the patient ID. This file was constructed with two different fitbit profiles which means that we need to filter through which data to track and which data to discard. We don't want to contribute a NFL Athlete's fitbit data to a Couch Potato or vice verca; that will throw off the averages and totals. We want the program to give accurate feedback and show correct results to the user.

---

To knock two birds with one stone, I created a `fileScan()` method to extract the data from the files and clean the data for use. The easiest problem to get around was the patient ID. Since we were already extracting a string, all I needed to do was compare the target ID to the patient ID using `strcmp()`. If they matched, then this is the data we need to clean. If it didn't, we don't care about it and move onto the next line of data.

The hard part of this assignment was cleaning the data itself. I couldn't use `strtok()` again to find holes in the data, and I couldn't use other library functions to find it for me either. I had to create a helper function to clean the data and format a given string. Take the broken line from above for example. In a text file, that would read as `12cx7,0:01:00,,0.0,0,,0,`. The holes were anywhere that there were two or more consecutive commas. If I could find out where two or more commas appeared then I'd know where my missing data was.

My approach to this was to create a buffer string and copy characters from the data line to the buffer. The program would march through the data line character by character, checking for two cases. Firstly, if there was a ',' right after another ',' we know that there is missing data between the two. The program would copy the first ',' and then insert a piece of invalid data such as a -1 or 49494949 depending on the data type. It would then `continue` the loop in search for other holes. The second case would be a ',' before a '\n' character, meaning that there is missing data at the end of the line. The program would do something similar for a hole in the middle of the data, except it would `break` out of the loop as we have already reached the end of the data line. If neither of these cases were met, the program would just copy the character directly to the buffer. Once we were finished, we would return the buffer and work off of that.

---

Once the data was cleaned, all we had to do was parse it and calculate averages and totals. I have never had to clean data before and wasn't quite sure how to do it. This whole project really opened my eyes to the field of Data Science and how to make sense of it. I always thought Data Scientists were people who really liked Excel and wanted a fancy job title, but I'm quickly being humbled by a little assignment. I actually had a lot of fun working out and planning this program! Sure there is a lot to improve, but I'm genuinely looking at delving into a career involving Data Science now.
