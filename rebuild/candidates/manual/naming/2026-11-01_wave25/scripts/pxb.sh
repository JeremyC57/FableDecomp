#!/bin/bash
# pxb.sh <ov> <addr...>: compile each row's overlay source; print errors only
S=${S:?}; source $S/wine/env.sh >/dev/null 2>&1; W=$S/naming/w25; ov=$1; shift
for a in "$@"; do r=$(python3 $W/scripts/bld.py $ov $a $W/pw/b_$a 2>&1); case "$r" in *.obj) echo "ok $a";; *) echo "== $a"; echo "$r" | head -4;; esac; done
