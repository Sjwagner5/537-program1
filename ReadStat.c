// Name: Sam Wagner
// File: ReadStat.c
// 
// This file is in charge of parsing the stat, statm, and status files. All of this information is found in a 
// similar way (use the pid to create the file path, scan in the file, extract and return the info) so it is 
// included in this file.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include "ReadStat.h"

/* 
* The getState function is called when the -s option is on. It will look through the /proc/[pid]/status file
* to get the current status of the process. It returns this to the driver file
*/
char getState(char *pid) {

	FILE *fp;
	int i = 0;
	char state[1];
	char path[25]; // This char array will hold the file path of the status file to scan

	int pathLength = snprintf(path, 21, "/proc/%s/status", pid);// Combine strings to create file path

	if (pathLength < 0) {
		printf("ERROR: could not get state of process %s", pid);
		exit(EXIT_FAILURE);
	}

	fp = fopen(path, "r");
	
	if (fp == NULL) {// Open the file and make sure there is no error
		printf("ERROR: could not get state of process %s", pid);
		exit(EXIT_FAILURE);
	}
	
	for (i = 0; i < 3; i++) {// Call scanf 3 times to skip over the first 2 entries in the file
		fscanf(fp, "%*s %s", state); 
	}

	fclose(fp);
	
	return state[0];
}

/*
* This method will get the user time taken up by a process. It takes a string of pid as an argument and 
* returns the user time from the /proc/[pid]/stat file
*/
int getUserTime(char *pid) {
	int utime = 0;
    FILE *fp;
    char path[25];// This char array will hold the file path of the stat file to scan

    int pathLength = snprintf(path, 19, "/proc/%s/stat", pid);// Combine strings to create file path

    if (pathLength < 0) {
        printf("ERROR: could not get user time of process %s", pid);
        exit(EXIT_FAILURE);
    }

	fp = fopen(path, "r");
	if (fp == NULL) {// Open the file and check for an error
		printf("ERROR: could not get user time of process %s", pid);
		exit(EXIT_FAILURE);
	}
	// Ignore the first 13 entries of the stat file to get to the user time
	fscanf(fp, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*i %*i %*i %*i %i", &utime);
	
    fclose(fp);

	return utime;
}

/*
* This method will be called to get the system time of the current user process. It takes the pid as an input
* and returns the system time from the /proc/[pid]/stat file
*/
int getSysTime(char *pid) {
	int systime = 0;
    FILE *fp;
    char path[25];// This char array will hold the file path of the stat file to scan

    int pathLength = snprintf(path, 25, "/proc/%s/stat", pid);// Combine strings to create file path
    
    if (pathLength < 0) {
        printf("ERROR: could not get system time of process %s", pid);
        exit(EXIT_FAILURE);
    }


	fp = fopen(path, "r");
	if (fp == NULL) {// Open the file and check for an error
		printf("ERROR: could not get system time of process %s", pid);
		exit(EXIT_FAILURE);
    }
     // Ignore the first 14 entries of the stat file to get to the system time
	fscanf(fp, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*i %*i %*i %*i %*i %i", &systime);

    fclose(fp);
	
	return systime;
}

/*
* This method will be used to get the virtual memory of a process. It takes the string of a pid as an argument
* and returns the virtual memory used in number of pages.
*/
int getVirtualMem(char *pid) {
	int vmem = 0;
    FILE *fp;
    char path[25];// This char array will hold the file path of the statm file to scan

    int pathLength = snprintf(path, 25, "/proc/%s/statm", pid);// Combine strings to create file path
    
    if (pathLength < 0) {
        printf("ERROR: could not get virutal memory of process %s", pid);
        exit(EXIT_FAILURE);
    }


	fp = fopen(path, "r");
	if (fp == NULL) {
		printf("ERROR: could not open /proc/[pid]/statm");
		exit(EXIT_FAILURE);
	}

	fscanf(fp, "%i", &vmem);// The first entry is virtual memory, so that gets scanned into the vmem variable
	
    fclose(fp);

	return vmem;
}
