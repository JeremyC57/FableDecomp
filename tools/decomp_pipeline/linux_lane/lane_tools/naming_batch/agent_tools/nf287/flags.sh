#!/bin/bash
# flags.sh <addr>: CompilerFlags of the row's catalog entry minus /O2 /Oy /W3 (q.py adds the base flags)
R=/home/user/FableDecomp
cf=$(grep -A14 "Address = '$1'" $R/rebuild/build_candidates.ps1 | grep -m1 -oP "CompilerFlags = '\K[^']+")
echo " $cf " | sed 's# /O2 # #g; s# /Oy # #g; s# /W3 # #g; s#^ *##; s# *$##'
