#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"

#include "config.h"
#include "utils.h"

/* Definition of all possible arguments */
ArgHook arg[] = {
   { "d",         0, argSetSearchOnDisk,  infoSetSearchOnDisk },
   { "h",         1, argSetStopHits,      infoSetStopHits },
   { "l",         1, argSetStopLength,    infoSetStopLength },
   { "m",         1, argSetMinWordLength, infoSetMinWordLength },
   { "M",         1, argSetMaxWordLength, infoSetMaxWordLength },
   { "q",         0, argSetQuiet,         infoSetQuiet },
   { "v",         0, argSetVerbose,       infoSetVerbose },
   { "w",         1, argSetStopWord,      infoSetStopWord },
   { "-help",     0, argPrintHelp,        infoPrintHelp },
   { "-version",  0, argPrintVersion,     infoPrintVersion },
   { {0} }
};

/* Makes sure the arguments passed actually make sense */
void argSanityCheck(void) {
   int xWordLen;
   
   if (minWordLength > maxWordLength) {
      fprintf(stderr, "%s : Minimum word length exceeds maximum word length%s", PROG_NAME, ENDL);
      exit(1);
   }

   if (quiet && verbose) {
      fprintf(stderr, "%s : Only one of -q and -v may be specified%s", PROG_NAME, ENDL);
      exit(1);
   }

   if (stopAfterWord) {
      xWordLen = strlen(xWord);

      if (CheckOnDisk(xWord, xWordLen) == 0) {
	 fprintf(stderr, "%s : Specified word is not in the dictionary%s", PROG_NAME, ENDL);
	 exit(1);
      }
      else if (xWordLen < minWordLength || xWordLen > maxWordLength) {
         fprintf(stderr, "%s : The specified word will never be found%s", PROG_NAME, ENDL);
         exit(1);
      }
   }

   if (stopAfterLength) {
      if (xLength < minWordLength || xLength > maxWordLength) {
         fprintf(stderr, "%s : A word of the specified length will never be found%s", PROG_NAME, ENDL);
         exit(1);
      }
   }
}

/* Goes through and parses all of the arguments.
 * Return value is index at which filenames begin
 * or zero if nothing is left on the command line */
short parseArgs(int argc, char **argv) {
   short i, j, found;
   char *szparam = 0;

   if (argc < MIN_ARGS) {
      fprintf(stderr, "%s : too few arguments%s", PROG_NAME, ENDL);
      exit(1);
   }

   for (i = 1; i < argc; i++) {
      szparam = argv[i];

      /* If the argument starts with a '-' it's a parameter flag */
      if (*szparam != '-') {
         /* Done parsing parameters, before returning do a sanity check */
         argSanityCheck();
         return i;
      }
      else
         szparam++;

      found = 0;
      j = 0;
      
      /* How dare they!  They didn't pass enough parameters! */
      if (argc < MIN_ARGS + 1) {
         fprintf(stderr, "%s : too few arguments%s", PROG_NAME, ENDL);
         exit(1);
      }

      /* Search for argument hooks */
      while (*arg[j].argName && !found) {
         /* Found one */
         if (strcmp(szparam, arg[j].argName) == 0) {
            if (arg[j].argHasArg) {
               if (i == argc - 1) {
                  fprintf(stderr, "%s : %s parameter is incomplete", PROG_NAME, szparam);
                  exit(1);
               }
               arg[j].argFunc(argv[i+1]);
               i++;
            }
            else
               arg[j].argFunc(0);
            found = 1;
         }

         j++;
      }

      /* If we didn't find the argument we better complain about it! */
      if (!found) {
         fprintf(stderr, "%s : invalid argument -%s%s", PROG_NAME, szparam, ENDL);
         exit(1);
      }
   }

   return 0;
}

/* SetSearchOnDisk argument functions */
void argSetSearchOnDisk(char* szparam) {
   searchOnDisk = 1;
}

void infoSetSearchOnDisk(void) {
   printf("Keep the dictionary on the disk when searching%s", ENDL);
}

/* SetMinWordLength argument functions */
void argSetMinWordLength(char* szparam) {
   minWordLength = atoi(szparam);
   if (minWordLength < DEF_MIN_WORD_LENGTH || minWordLength > DEF_MAX_WORD_LENGTH) {
      fprintf(stderr, "%s : Invalid minimum word length%s", PROG_NAME, ENDL);
      exit(1);
   }
}

void infoSetMinWordLength(void) {
   printf("Sets the minimum word length (default = %d).%s", DEF_MIN_WORD_LENGTH, ENDL);
}

/* SetMaxWordLength argument functions */
void argSetMaxWordLength(char* szparam) {
   maxWordLength = atoi(szparam);
   if (maxWordLength < DEF_MIN_WORD_LENGTH || maxWordLength > DEF_MAX_WORD_LENGTH) {
      fprintf(stderr, "%s : Invalid maximum word length%s", PROG_NAME, ENDL);
      exit(1);
   }
}

void infoSetMaxWordLength(void) {
   printf("Sets the maximum word length (default = %d).%s", DEF_MAX_WORD_LENGTH, ENDL);
}

/* SetStopHits argument functions */
void argSetStopHits(char* szparam) {
   stopAfterHits = 1;
   
   xHits = atol(szparam);
   if (xHits <= 0) {
      fprintf(stderr, "%s : Invalid value for parameter -h%s", PROG_NAME, ENDL);
      exit(1);
   }
}

void infoSetStopHits(void) {
   printf("Stop after arg words are generated.%s", ENDL);
}

/* SetStopWord argument functions */
void argSetStopWord(char* szparam) {
   short paramLength = strlen(szparam);

   stopAfterWord = 1;
   if (paramLength < DEF_MIN_WORD_LENGTH) {
      fprintf(stderr, "%s : Specified word is too short%s", PROG_NAME, ENDL);
      exit(1);
   }
   else if (paramLength > DEF_MAX_WORD_LENGTH) {
      fprintf(stderr, "%s : Specified word is too long%s", PROG_NAME, ENDL);
      exit(1);
   }

   strcpy(xWord, szparam);
}

void infoSetStopWord(void) {
   printf("Stop after specified word is generated.%s", ENDL);
}

/* SetStopLength argument functions */
void argSetStopLength(char* szparam) {
   stopAfterLength = 1;
   
   xLength = atoi(szparam);
   if (xLength < DEF_MIN_WORD_LENGTH || xLength > DEF_MAX_WORD_LENGTH) {
      fprintf(stderr, "%s : Specified length is invalid%s", PROG_NAME, ENDL);
      exit(1);
   }
}

void infoSetStopLength(void) {
   printf("Stop after arg length word is found.%s", ENDL);
}

/* SetQuiet argument functions */
void argSetQuiet(char* szparam) {
   quiet = 1;
}

void infoSetQuiet(void) {
   printf("Quiet mode.  Only print words found in the dictionary.%s", ENDL);
}

/* SetVerbose argument functions */
void argSetVerbose(char *szparam) {
   verbose = 1;
}

void infoSetVerbose(void) {
   printf("Verbose mode.  Print all randomly generated strings.%s", ENDL);
}

/* Help argument functions */
void argPrintHelp(char* szparam) {
   unsigned short i, j;
   
   printf("Usage : %s [options] filename%s%s", PROG_NAME, ENDL, ENDL);
   printf("Displays a list of randomly generated words.%s%s", ENDL, ENDL);

   i = 0;
   while (*arg[i].argName) {
      if (arg[i].argHasArg) {
         printf("  -%s arg", arg[i].argName);
         for (j = 0; j < 12 - strlen(arg[i].argName); j++)
            printf(" ");
      }
      else
         printf("  -%-10s      ", arg[i].argName);
      arg[i].argInfo();
      i++;
   }

   printf("%s", ENDL);
   printf("Send bugs and comments to %s <%s>%s", AUTH_NAME, AUTH_EMAIL, ENDL);

   exit(0);
}

void infoPrintHelp(void) {
   printf("Displays this help message and exits.%s", ENDL);
}

/* Version argument functions */
void argPrintVersion(char* szparam) {
   printf("%s %s.%s%s", PROG_NAME, VER_MAJOR, VER_MINOR, ENDL);
   exit(0);
}

void infoPrintVersion(void) {
   printf("Displays %s's version information and exits.%s", PROG_NAME, ENDL);
}
