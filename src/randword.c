#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#include "config.h"
#include "args.h"
#include "utils.h"

long numfound = 0;
long totalcount = 0;

int main(int argc, char **argv) {
   char randword[DEF_MAX_WORD_LENGTH + 1];
   int randlen;
   short i, stop = 0;

   /* Parse the command line arguments */
   parseArgs(argc, argv);

   /* Seed the random number generator */
   srand(time(NULL));

   /* If something goes wrong in init, it'll take care of itself */
   InitProgram();

   /* Register exit function */
   signal(SIGINT, SigHandler);
   atexit(ExitProgram);

   /* Start generate strings */
   while (!stop) {
      randlen = rand() % (maxWordLength - minWordLength + 1) + minWordLength;

      for (i = 0; i < randlen; i++)
         randword[i] = (unsigned char)(rand() % 26 + 97);

      randword[randlen] = '\0';

      totalcount++;
      
      if (CheckWord(randword, randlen)) {
         numfound++;

         printf("%s", randword);
         if (verbose)
            printf(" *");
         printf("%s", ENDL);

         if (stopAfterLength && randlen == xLength)
            stop = 1;
      }
      else if (verbose)
         printf("%s%s", randword, ENDL);

      if (stopAfterWord && strcmp(xWord, randword) == 0)
         stop = 1;

      if (stopAfterHits && numfound == xHits)
         stop = 1;
   }
   
   return 0;
}

