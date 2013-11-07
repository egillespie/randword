# randword readme

This package contains the full source code for the randword random
word generator.  The use of this software is granted upon the
agreement to give the original author credit where applicable.  In
other words, if you use my code, let me and the people using your
software know you did!

If you implement a new feature and want me to merge it into my
version send me what I need to know to make the changes including
your name and email so I can give you appropriate credit as well.

Enjoy!

## About This Program

This tool is something similar to a monkey sitting at a keyboard
pressing random characters.  Given the constraints passed to the
program from the command line the application first randomly decides
how long the next randomly generated string will be.  Then the program
adds random characters to a string of the predetermined length until
the string is full.  Finally, the application checks the string against
its dictionaries to determine if the randomly generated string is an
actual word in the English language.

Of note, randword was initially created in 1997 by me (Erik Gillespie)
and Keith Batten.  I have completely overhauled the program since then
to add more functionality, restructure and convert the code from C++ to
C (C++ is my best guess, it was clearly newbie code :).

In order for randword to run correctly the dictionary files must exist
in a subfolder called 'dict' relative to the current working directory.
The dictionary files must be named word03.txt, word04.txt, ..., 
word20.txt.  The dictionaries are expected to contain words of a certain
length and those words must be all lower-case ASCII and each on a line
of their own with only a newline at the end of each line, no carriage
return/linefeed pairs.  Go to my homepage (see the link at the bottom of
this document) to download a sample dictionary.

Here's the output of randword's --help option:

    Usage : randword [options] filename

    Displays a list of randomly generated words.

      -d               Keep the dictionary on the disk when searching
      -h arg           Stop after arg words are generated.
      -l arg           Stop after arg length word is found.
      -m arg           Sets the minimum word length (default = 3).
      -M arg           Sets the maximum word length (default = 20).
      -q               Quiet mode.  Only print words found in the dictionary.
      -v               Verbose mode.  Print all randomly generated strings.
      -w arg           Stop after specified word is generated.
      --help           Displays this help message and exits.
      --version        Displays randword's version information and exits.

Send bugs and comments to Erik Gillespie <erik.gillespie@gmail.com>.

If you're curious about some of the technical features in the code,
randword runs in Linux and DOS (I've only tested in Windows 2000's 
command prompt however).  The dictionaries can be searched on the disk
or all in memory using a binary search.  The binary search is pretty fast
but searching on the disk is much slower than searching in memory.  Also,
if you run randword with no arguments then randword will run indefinitely
if you let it.  randword does catch the interrupt signal (SIGINT) though
so in the correct environment (again I've only tested in Linux and
Windows 2000's command prompt) you can press Ctrl-C to terminate the
program and randword will exit gracefully, reporting the number of random
strings generated, the number of words found, and the number of non-words
found.

## Changes Since v1.0

* All needed dictionaries are preloaded into memory and searched there
  instead of from the disk.  I noticed a 13-14 time speed-up by doing
  this on my computer.
* Added option '-d' to use old method of searching dictionaries on the
  disk.  randword uses less memory that way in case you're running on
  an older computer or when you just need the extra resources.
* The Win32 version of randword outputs carriage-return/line-feed pairs
  instead of just line-feeds.  In any other OS randword will output
  line-feeds.
* Fixed an incorrect technical detail in this document.


## Contact Information

If you want to get a hold of me to request a feature, put in a bug
report, make comments (I like getting emails that people actually use
my programs :) then send me an email at <erik.gillespie@gmail.com>.

You can always get the newest version of randword on Github:
<http://github.com/egillespie/>

