#ifndef __READPROC_H__
#define __READPROC_H__

bool checkPidDigits(char *pid);
bool isUserProc(struct dirent **userProcs);
char* getCmdLine(char *pid);

#endif
