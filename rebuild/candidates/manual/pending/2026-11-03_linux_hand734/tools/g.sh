#!/bin/bash
# g.sh <addr> <src> [leaf] [extra q.py args]: include check, cpp check, then grade against the full mirror
export S=${S}
source $S/wine/env.sh; export S
W=$S/w22/chunk734
cd /home/user/FableDecomp
a=$1; src=$2; shift 2
m=$(python3 $S/w22/agent724b/incres.py $src $W/mirror | grep MISSING | grep -v "MISSING <\|stddef.h\|string.h\|stdlib.h\|math.h\|new\b" )
if [ -n "$m" ]; then echo "$m"; fi
cpp -P -nostdinc -undef -D_MSC_VER=1310 -D_M_IX86=600 -D_WIN32 -I $W/mirror -I $S/stubinc $src -o /dev/null 2>&1 | grep -E "#error|error:" | grep -v "No such file" | head -5
export FABLE_HDR_OVERLAY=$W/mirror
timeout 120 python3 $S/tools/q.py $a $src "$@"
