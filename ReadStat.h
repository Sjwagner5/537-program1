#ifndef __READSTAT_H__
#define __READSTAT_H__

char getState(char *pid);
int getUserTime(char *pid);
int getSysTime(char *pid);
int getVirtualMem(char *pid);

#endif 
