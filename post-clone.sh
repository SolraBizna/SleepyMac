#!/bin/sh
# Run this script from MacRelix after a fresh `git clone` to restore all the Finder
# attributes that should be here, and produce the resource forks that are required.

SetFile -t 'TEXT' -c 'MPS ' *.c *.h *.a *.sh MakeFile .gitattributes .gitignore
SetFile -t 'OBJ ' -c 'MPS ' *.o
# MacRelix SetFile lacks -a
#SetFile -a V .git Objects/.keep
macbin decode "SleepyMac.rsed.bin"
macbin decode "SleepyMac Read Me.bin"
