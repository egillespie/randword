#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "utils.h"

#include "config.h"

/* Yes, that's three *'s, a triple pointer! */
static char ***dict = NULL;
static unsigned long wordsOnPage[DEF_MAX_WORD_LENGTH];

void SigHandler(int sig) {
   if (sig == SIGINT)
      exit(0);
}

void InitProgram(void) {
   FILE *fd = NULL;
   char fname[16];
   int i, npages, wordlen;
   unsigned long j;

   /* Load the dictionary into memory if we're not searching on the disk */
   if (!searchOnDisk) {
      npages = maxWordLength - minWordLength + 1;

      /* Allocate memory for however many pages we're going to use */
      dict = (char***)malloc(npages * sizeof(char**));
      if (dict == NULL) {
	 fprintf(stderr, "%s : Error allocating space for dictionaries%s", PROG_NAME, ENDL);
	 exit(1);
      }

      /* For each page, allocate space for all of the words on that page */
      for (i = 0; i < npages; i++) {
	 /* First, initialize the pointer to NULL */
	 dict[i] = NULL;

	 /* Get the word length for this page */
	 wordlen = minWordLength + i;

	 /* Open the appropriate dictionary file */
	 sprintf(fname, "dict%cword%02d.txt", DIR_SLASH, wordlen);
	 fname[15] = '\0';

	 if ((fd = fopen(fname, "r")) == NULL) {
	    fprintf(stderr, "%s : Error opening dictionary file '%s'%s", PROG_NAME, fname, ENDL);
	    exit(1);
	 }

	 /* Figure out how many words are in the file */
	 wordsOnPage[i] = FileSize(fd) / (wordlen + 2);

	 /* Allocate space for that many words */
	 dict[i] = (char**)malloc(wordsOnPage[i] * sizeof(char*));
	 if (dict[i] == NULL) {
	    fprintf(stderr, "%s : Error allocating space for page %d%s", PROG_NAME, wordlen, ENDL);
	    fclose(fd);
	    exit(1);
	 }

	 /* Allocate space for each word */
	 for (j = 0; j < wordsOnPage[i]; j++) {
	    /* First, initialize to NULL */
	    dict[i][j] = NULL;

	    /* When allocating, remember space for the null terminator */
	    dict[i][j] = (char*)malloc((wordlen + 1) * sizeof(char));
	    if (dict[i][j] == NULL) {
	       fprintf(stderr, "%s : Error allocating space for page %d, word %ld%s", PROG_NAME, wordlen, j + 1, ENDL);
	       fclose(fd);
	       exit(1);
	    }

	    /* Read the word from the file into memory */
	    fseek(fd, j * (wordlen + 2), SEEK_SET);
	    fread(dict[i][j], wordlen, 1, fd);
	    dict[i][j][wordlen] = '\0';
	 }

	 /* Done loading that page, close the file */
	 fclose(fd);
      }
   }

   /* Print some initial report information */
   if (!quiet) {
      printf("Minimum Word Length: %d%s", minWordLength, ENDL);
      printf("Maximum Word Length: %d%s", maxWordLength, ENDL);
      
      if (stopAfterHits || stopAfterWord || stopAfterLength)
	 printf("%s", ENDL);

      if (stopAfterHits)
         printf("Stopping after %ld words are found in the dictionary.%s", xHits, ENDL);
      if (stopAfterWord)
         printf("Stopping after %s is randomly generated.%s", xWord, ENDL);
      if (stopAfterLength)
         printf("Stopping after word of length %d is generated.%s", xLength, ENDL);

      printf("%s", ENDL);
   }
}

void ExitProgram(void) {
   int i;
   unsigned long j;
   short stop = 0;

   if (!quiet) {
      printf("%s", ENDL);
      printf("Tries:  %ld%s", totalcount, ENDL);
      printf("Hits:   %ld%s", numfound, ENDL);
      printf("Misses: %ld%s", totalcount - numfound, ENDL);
   }

   if (!searchOnDisk) {
      /* Free up all of that memory */
      if (dict != NULL) {
	 for (i = 0; i < maxWordLength - minWordLength + 1 && !stop; i++) {
	    if (dict[i] != NULL) {
	       for (j = 0; j < wordsOnPage[i]; j++) {
		  if (dict[i][j] != NULL)
		     free(dict[i][j]);
		  else {
		     stop = 1;
		     break;
		  }
	       }

	       free(dict[i]);
	    }
	    else
	       break;
	 }
	 free(dict);
      }
   }
}

/* Returns the number of bytes in a file */
unsigned long FileSize(FILE *fd) {
   unsigned long curpos, length;
   
   curpos = ftell(fd);
   fseek(fd, 0, SEEK_END);
   length = ftell(fd);
   fseek(fd, curpos, SEEK_SET);
   
   return length;
}

int CheckWord(const char *aword, int buflen) {
   //return CheckOnDisk(aword, buflen);
   return (searchOnDisk ? CheckOnDisk(aword, buflen) : CheckInMem(aword, buflen));
}

/* TODO: This function will NOT work with UNIX files.  It expects a carriage return
 *       after each word.  This should probably be fixed. */
int CheckOnDisk(const char *aword, int buflen) {
   char filestr[DEF_MAX_WORD_LENGTH + 1];
   char tfname[16];
   long lwr, upr, cur;
   int cmp;
   FILE *fin;

   sprintf(tfname, "dict%cword%02d.txt", DIR_SLASH, buflen);
   tfname[15] = '\0';

   if ((fin = fopen(tfname, "r")) == NULL) {
      fprintf(stderr, "%s : Error opening dictionary file '%s'%s", PROG_NAME, tfname, ENDL);
      exit(1);
   }

   lwr = -1;
   upr = (FileSize(fin) / (buflen + 2)) + 1;
   cur = (upr + lwr) / 2;
   
   while ((cur != upr) && (cur != lwr)) {
      fseek(fin, cur * (buflen + 2), SEEK_SET);
      fread(filestr, buflen, 1, fin);
      filestr[buflen] = '\0';

      cmp = strncmp(aword, filestr, buflen);

      if (cmp == 0) {
         fclose(fin);
         return 1;
      }
      else if (cmp > 0)
         lwr = cur;
      else
         upr = cur;

      cur = (lwr + upr) / 2;
   }

   fclose(fin);

   return 0;
}

int CheckInMem(const char *aword, int buflen) {
   static long lwr, upr, cur;
   register int cmp;
   register int idx = buflen - minWordLength;
   

   lwr = -1;
   upr = wordsOnPage[idx];
   cur = (upr + lwr) / 2;

   while ((cur != upr) && (cur != lwr)) {
      cmp = strncmp(aword, dict[idx][cur], buflen);

      if (cmp == 0)
	 return 1;
      else if (cmp > 0)
	 lwr = cur;
      else
	 upr = cur;

      cur = (lwr + upr) / 2;
   }

   return 0;
}
