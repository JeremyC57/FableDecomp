#!/bin/bash
# fixup.sh <mcheck log> <outlog>: tdfix + eetfix + enumfix on the log's SRC_FAIL rows, then mcheck them into <outlog>
export S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad
source $S/wine/env.sh >/dev/null; export FABLE_ROOT=$S/mig9/repo; cd $S/mig9/repo
F=$(grep -P '\tSRC_FAIL' $1 | cut -f1 | while read a; do ls rebuild/src/compiled/*/*/*_$a.cpp; done)
[ -z "$F" ] && exit 0
python3 $S/mig9/scripts/tdfix.py $F > /dev/null 2>&1
python3 $S/mig9/scripts/eetfix.py $F > /dev/null 2>&1
timeout 1500 python3 $S/mig9/scripts/enumfix.py $F > /dev/null 2>&1
timeout 3000 python3 $S/mig9/scripts/mcheck.py -j 3 $F > $2 2>&1
cut -f2 $2 | sort | uniq -c
