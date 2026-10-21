#!/bin/bash
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; cd /home/user/FableDecomp
W=$S/w22/retry10; out=$W/s7/tbsweep_r.tsv
for a in $(cat $W/s7/tb_rerun.txt); do
  r=$(python3 $W/s7/tmplbody.py $a s7tb 2>&1 | tail -1)
  case "$r" in var/*) ;; *) continue;; esac
  b=$(python3 $W/var/pv.py $a $W/src/$a.cpp 2>&1 | tail -1 | cut -c1-40)
  v=$(python3 $W/var/pv.py $a $W/var/${a}_s7tb.cpp 2>&1 | tail -1 | cut -c1-80)
  echo -e "$a\tbase=$b\ttb=$v\t${r#* }" >> $out
done
echo DONE >> $out
