#ifndef ARGS_H
#define ARGS_H

/* Minimum number of arguments required for the program to run */
#define MIN_ARGS        0

/* Associates argument strings with their corresponding functions */
typedef struct ArgHook {
   char  argName[10];
   short argHasArg;
   void  (*argFunc)(char*);
   void  (*argInfo)(void);
} ArgHook, *pArgHook;

/* The table of all possible arguments */
extern ArgHook arg[];

/* Functions used for "parsing" arguments */
void  argSanityCheck(void);
short parseArgs(int, char**);

/* All possible argument functions */
void argSetSearchOnDisk(char*);
void argSetMinWordLength(char*);
void argSetMaxWordLength(char*);
void argSetStopHits(char*);
void argSetStopWord(char*);
void argSetStopLength(char*);
void argSetQuiet(char*);
void argSetVerbose(char*);
void argPrintHelp(char*);
void argPrintVersion(char*);

/* Each argument is also required to provide help about what it does */
void infoSetSearchOnDisk(void);
void infoSetMinWordLength(void);
void infoSetMaxWordLength(void);
void infoSetStopHits(void);
void infoSetStopWord(void);
void infoSetStopLength(void);
void infoSetQuiet(void);
void infoSetVerbose(void);
void infoPrintHelp(void);
void infoPrintVersion(void);

#endif
