// Name: Sam Wagner
// File: driver.c
//
// This file serves as the main driver for the rest of the program. It parses, the command line, processes the
// inputs and calls the corresponding functions needed to get the information being requested. It also prints
// the output that is being requested.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include "ReadProc.h"
#include "ReadStat.h"
#define __GNU_SOURCE


/*
* This is the main driver function for the entire program. It handles the inputs, calls the correct functions,
* and prints the correct output
*/
int main(int argc, char* argv[]) {
		
	// These flags will help determine which options have been chosen by the user. They are set to their
	// default values
	bool pflag = false;
	bool sflag = false;
	bool uflag = true;
	bool vflag = false;
	bool cflag = true;
	bool capSflag = false;

	char *pid = malloc(sizeof(char)*8);// The current PID we are looking at
	char state[1];// Process state
	int utime;// User time
	int systime;// System time
	int vmem;// Virtual memory
	char *cmdline;// Command line for a process
	int numProcs;// This will store the number of processes we need to display info for
	int options;// The options chosen by the user (returned from getopt())
	struct dirent *userProcs[250];// An array of pointers pointing to the user's processes
	char argument[30];// Stores the arguments for each option if there is any
	
	for (int i = 0; i < 250; i++) {
		userProcs[i] = malloc(sizeof(struct dirent*));
	}

	// All options except for p have an optional arg to handle a 2nd - to turn them off
	while ((options = getopt(argc, argv, "p:s::U::v::c::S::")) != -1) {

		if (options == '?') {
			exit(EXIT_FAILURE);
		}
		
		switch(options) {// Parse options chosen by user
		case 'p':
			pflag = true;
			int pidlength = snprintf(pid, 8, "%s", optarg);
			if (pidlength <= 0) {
				printf("ERROR: could not read pid\n");
				exit(EXIT_FAILURE);
			}
			if (!checkPidDigits(pid)) {
				printf("ERROR: <pid> must only contain integers\n");
				exit(EXIT_FAILURE);
			}
			
			
			break;
		case 's':
			if (optarg == 0) {// If no arg
				sflag = true;
			} else {
				int arglen = snprintf(argument, 5, "%s", optarg);
				if (arglen < 0) {
					printf("ERROR: could not parse argument for -s\n");
					exit(EXIT_FAILURE);

				} else if (strcmp(argument, "-") == 0) {// If user turns option off
					sflag = false;

				} else {
					printf("ERROR: options for state flag are -s or -s-\n");
					exit(EXIT_FAILURE);
				}
			}	
			break;
		case 'U':
            if (optarg == 0) {
                uflag = true;
            } else {
                int arglen = snprintf(argument, 5, "%s", optarg);
				if (arglen < 0) {
                    printf("ERROR: could not parse argument for -U\n");
                    exit(EXIT_FAILURE);

                } else if (strcmp(argument, "-") == 0) {
                    uflag = false;

                } else {
                    printf("ERROR: options for user time flag are -U or -U-\n");
                    exit(EXIT_FAILURE);
                }
            }

			break;
		case 'v': 
            if (optarg == 0) {
                vflag = true;
            } else {
                int arglen = snprintf(argument, 5, "%s", optarg);

                if (arglen < 0) {
                    printf("ERROR: could not parse argument for -v\n");
                    exit(EXIT_FAILURE);

                } else if (strcmp(argument, "-") == 0) {
                    vflag = false;

                } else {
                    printf("ERROR: options for virtual memory flag are -v or -v-\n");
                    exit(EXIT_FAILURE);
                }
            }

			break;
		case 'c':
            if (optarg == 0) {
                cflag = true;
            } else {
                int arglen = snprintf(argument, 5, "%s", optarg);
                if (arglen < 0) {
                    printf("ERROR: could not parse argument for -c\n");
                    exit(EXIT_FAILURE);

                } else if (strcmp(argument, "-") == 0) {
                    cflag = false;

                } else {
                    printf("ERROR: options for cmdline flag are -c or -c-\n");
                    exit(EXIT_FAILURE);
                }
            }

			break;
		case 'S':
            if (optarg == 0) {
                capSflag = true;
            } else {
                int arglen = snprintf(argument, 5, "%s", optarg);
                if (arglen < 0) {
                    
					printf("ERROR: could not parse argument for -S\n");
                    exit(EXIT_FAILURE);

                } else if (strcmp(argument, "-") == 0) {
                    capSflag = false;

                } else {
                    printf("ERROR: options for system time flag are -S or -S-\n");
                    exit(EXIT_FAILURE);
                }
            }

			break;
		}			
	
	}
	
	numProcs = 0;
	DIR *directory;
	struct dirent *item = NULL;// Pointer to the different items of /proc
		
	if ((directory = opendir("/proc")) == NULL) {// Open /proc directory
		printf("ERROR: could not open /proc directory\n");
		exit(EXIT_FAILURE);
		
	} else {
		while ((item = readdir(directory)) != NULL) {// Parse through /proc directory
			if (pflag) {
				if (strcmp(pid, item->d_name) == 0) {// If -p is present we must check if the PID is present
													 // in /proc
				
					numProcs = 1;// If it is, we have 1 process to print output for. Otherwise numProcs stays at
								 // 0
					break;
				}

			} else if (isUserProc(&item)) {// If -p is not present, we call isUserProc() for each entry in the
										   // /proc directory to see if it belongs to the current user
				
				userProcs[numProcs] = item;// If it is a user process, we store the pointer to the struct for
										   // that item in the userProcs array
				numProcs++;
			}
		}
	}
	
	for (int i = 0; i < numProcs; i++) {// Print output for all processes requested
		if (!pflag) {
			int pidLength = snprintf(pid, 30, "%s", userProcs[i]->d_name);
			if (pidLength < 0) {
				printf("ERROR: could not process a PID from /proc");
				exit(EXIT_FAILURE);
			}
		}
		printf("%s: ", pid);
		if (sflag) {
			state[0] = getState(pid);
			printf("%c ", state[0]);	
		}
		if (uflag) {
			utime = getUserTime(pid);
			printf("utime=%i ", utime);
		}
		if (capSflag) {
			systime = getSysTime(pid);
			printf("systime=%i ", systime);
		}
		if (vflag) {
			vmem = getVirtualMem(pid);
			printf("vmem=%i ", vmem);
		}
		if (cflag) {
			cmdline = getCmdLine(pid);
			printf("[ %s]", cmdline);
		}	
		printf("\n");
	}

	free(pid);	
	free(cmdline);// Memory for the cmdline pointer is malloced in the getCmdLine function, so it must be freed
}
