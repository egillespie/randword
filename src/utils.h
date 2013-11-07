#ifndef RANDWORD_UTILS_H
#define RANDWORD_UTILS_H

#include <stdio.h>
#include <stdlib.h>

extern long numfound;
extern long totalcount;

void SigHandler(int);
void InitProgram(void);
void ExitProgram(void);
unsigned long FileSize(FILE*);

int CheckWord(const char*, int);
int CheckOnDisk(const char*, int);
int CheckInMem(const char*, int);

#endif
