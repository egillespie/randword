PROJNAME = randword

# -------------------------------------------
# Define some directories
# -------------------------------------------
SRCDIR = src

# -------------------------------------------
# Define some of the tools we may use
# -------------------------------------------

LD      = gcc
CC      = gcc
RM      = rm -f

# -------------------------------------------
# Define the flags for the compilers
# -------------------------------------------
OPTFLAG = -O3
CFLAGS  = -I$(SRCDIR) $(OPTFLAG) -c -Wall
LDFLAGS = -lc

# -------------------------------------------
# Define the list(s) of all files needed
# -------------------------------------------
O_FILES =	args.o		\
		config.o	\
		randword.o	\
		utils.o

# -------------------------------------------
# Object targets/dependencies
# -------------------------------------------
all : $(PROJNAME)

debug : OPTFLAG = -g
debug : $(PROJNAME)

$(PROJNAME) : $(O_FILES)
	$(LD) -o $@ $(O_FILES) $(LDFLAGS)

%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $< 

# -------------------------------------------
# Miscellaneous targets
# -------------------------------------------

cleanobj : 
	$(RM) $(O_FILES)

clean : cleanobj
	$(RM) $(PROJNAME)
