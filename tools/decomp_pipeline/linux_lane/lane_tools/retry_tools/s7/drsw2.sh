#!/bin/bash
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; cd /home/user/FableDecomp
W=$S/w22/retry10; out=$W/s7/drsweep2.tsv
for a in $(cat $W/s7/dr2_cands.txt); do
  line="$a"
  for m in addref clear; do
    r=$(python3 $W/s7/defreal.py $a s7d$m $m 2>&1 | tail -1)
    case "$r" in var/*) v=$(python3 $W/var/pv.py $a $W/var/${a}_s7d$m.cpp 2>&1 | tail -1 | cut -c1-60);; *) v="skip";; esac
    line="$line\t$m=$v"
  done
  echo -e "$line" >> $out
done
echo DONE >> $out
