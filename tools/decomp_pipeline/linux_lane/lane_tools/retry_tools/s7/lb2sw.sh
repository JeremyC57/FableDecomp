#!/bin/bash
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; cd /home/user/FableDecomp
W=$S/w22/retry10; out=$W/s7/lb2sweep.tsv
for a in $(cat $W/s7/lb2_cands.txt); do
  b=$(python3 $W/var/pv.py $a $W/src/$a.cpp 2>&1 | tail -1 | cut -c1-40)
  v=$(python3 $W/var/pv.py $a $W/var/${a}_s7lb2.cpp 2>&1 | tail -1 | cut -c1-80)
  echo -e "$a\tbase=$b\tlb=$v" >> $out
done
echo DONE >> $out
