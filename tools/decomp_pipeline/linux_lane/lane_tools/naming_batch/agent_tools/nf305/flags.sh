#!/bin/bash
# flags.sh <addr>: CompilerFlags of the row's catalog entry minus /O2 /Oy /W3 (q.py adds the base flags).
# NF305 fix: read only the row's own [pscustomobject] block (the old grep -A14 could pick up the NEXT entry's flags).
R=/home/user/FableDecomp
cf=$(awk -v a="Address = '$1'" 'index($0,a){f=1} f&&/^ *}/{exit} f' $R/rebuild/build_candidates.ps1 | grep -m1 -oP "CompilerFlags = '\K[^']+")
echo " $cf " | sed 's# /O2 # #g; s# /Oy # #g; s# /W3 # #g; s#^ *##; s# *$##'
