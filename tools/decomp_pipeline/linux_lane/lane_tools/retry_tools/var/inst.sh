#!/bin/bash
# inst.sh <addr> <suffix>: install var/<addr>_<suffix>.cpp as src/<addr>.cpp (Grade lines dropped, original backed up) and grade it
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; cd /home/user/FableDecomp
W=$S/w22/retry10; a=$1; v=$2
[ -f $W/var/${a}_orig.cpp ] || cp $W/src/$a.cpp $W/var/${a}_orig.cpp
grep -v "^// Grade:" $W/var/${a}_$v.cpp > $W/src/$a.cpp
leaf=$(grep -m1 "^// Leaf:" $W/src/$a.cpp | sed 's|// Leaf: *||'); [ -z "$leaf" ] && leaf=$(grep -P "^$a\t" $W/rows.tsv | cut -f3)
bash $W/g.sh $a "$leaf" | head -1
