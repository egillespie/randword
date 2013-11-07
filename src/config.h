#ifndef CONFIG_H
#define CONFIG_H

#include <limits.h>

/* Compile-time configuration */

#define PROG_NAME          "randword"

#define VER_MAJOR          "1"
#define VER_MINOR          "1"

#define AUTH_NAME          "Erik Gillespie"
#define AUTH_EMAIL         "erik.gillespie@gmail.com"

#ifdef WIN32
# define DIR_SLASH         '\\'
# define ENDL              "\r\n"
#else
# define DIR_SLASH         '/'
# define ENDL              "\n"
#endif

#define DEF_MIN_WORD_LENGTH   3
#define DEF_MAX_WORD_LENGTH   20

/* Run-time configuration */

short searchOnDisk;

int   minWordLength;
int   maxWordLength;

short stopAfterHits;
short stopAfterWord;
short stopAfterLength;

long  xHits;
char  xWord[DEF_MAX_WORD_LENGTH + 1];
short xLength;

short quiet;
short verbose;

#endif
