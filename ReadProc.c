// Name: Sam Wagner
// File: ReadProc.c
// 
// This file contains functions to help extract info from the /proc directory

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include "ReadProc.h"

/*
* This function is used to make sure that a PID only contains integers. It accepts a char pointer to the PID as
* an argument and returns true if it only contains integers and false otherwise
*/
bool checkPidDigits(char *pid) {
	for (int i = 0; i < strnlen(pid, (sizeof(char)*7)); i++) {// The max length is 7 because that is the max
															  // length of a PID
    	if ((int)pid[i] > 57 || (int)pid[i] < 48) {// Check ascii value
   	   		return false;
        }
    }
	return true;
}

/*
* This function is used to determine if a process belongs to the user. It takes a pointer to a dirent
* structure as an argument. It then gets the PID of a process from that structure. Finally, it parses that 
* process's status file, gets it's real UID, and compares it to the current user's ID. If they match, it returns
* true, otherwise it returns false
*/
bool isUserProc(struct dirent **item) {

	char path[30];
	char procUid[30];
	char myUid[30];
	FILE *fp;

	if(!checkPidDigits((*item)->d_name)) {
		return false;
	}

	int uidLength = snprintf(myUid, 30, "%i", getuid());	
	
	if (uidLength < 0) {
		printf("ERROR: could not get UID");
		exit(EXIT_FAILURE);
	}

	int pathLength = snprintf(path, 30, "/proc/%s/status", (*item)->d_name);

	if (pathLength < 0) {
		printf("ERROR: could not get file path for /status file for process %s", (*item)->d_name);
		exit(EXIT_FAILURE);
	}

    fp = fopen(path, "r");
                
	for (int i = 0; i < 9; i++) {// Skip entries in the file until we get to the UID
                    
		fscanf(fp, "%*s %*s %s", procUid);
    }
	fclose(fp);
	
    return (strcmp(myUid, procUid) == 0);
}

/*
* This function gets the command line for a process. It takes a pid as an argument, parses through its cmdline
* file, and returns that as a string to the user. 
*/
char* getCmdLine(char *pid){
	
	FILE *fp;
	char *cmdline = malloc(250*sizeof(char));
	char path[25];
	
	int pathlength = snprintf(path, 25, "/proc/%s/cmdline", pid);
	
	if (pathlength < 0) {
		printf("ERROR: could not get the command line for the process %s\n", pid);
		exit(EXIT_FAILURE);
	}
	
	fp = fopen(path, "r");
	
	if (fp == NULL) {
		printf("ERROR: cannot open /proc/%s/cmdline\n", pid);
		exit(EXIT_FAILURE);
	}
		
	int i = 0;
	char curr;
	
	while((curr = fgetc(fp)) != EOF) {// Read char by char until we reach the end of the file
		if (curr == 0) {
			cmdline[i] = ' ';// Replace null terminators with spaces
		} else {
			cmdline[i] = curr;
		}
		
		i++;
	}	
	
	fclose(fp);
	
	return cmdline;
}
