#include "config.h"

/* Set default values for global config variables */

short searchOnDisk = 0;

int   minWordLength = DEF_MIN_WORD_LENGTH;
int   maxWordLength = DEF_MAX_WORD_LENGTH;

short stopAfterHits = 0;
short stopAfterWord = 0;
short stopAfterLength = 0;

long  xHits = 0;
char  xWord[DEF_MAX_WORD_LENGTH + 1] = "";
short xLength = 0;

short quiet = 0;
short verbose = 0;
