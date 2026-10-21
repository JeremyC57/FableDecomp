#!/bin/bash
# pva.sh <addr> <abs-or-rel-file>...: pv.py diff-line count for variant files (any path)
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
a=$1; shift; files=(); for f in "$@"; do files+=("$(realpath $f)"); done; cd /home/user/FableDecomp
for f in "${files[@]}"; do echo "$a $(basename $f): $(python3 $S/w22/retry10/var/pv.py $a $f 2>&1 | tail -1)"; done
