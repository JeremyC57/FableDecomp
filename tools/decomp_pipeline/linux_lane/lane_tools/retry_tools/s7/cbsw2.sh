#!/bin/bash
W=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/retry10
for a in $(cat $W/s7/tb_cands.txt); do s=$(grep -P "^$a\t" $W/STATUS.tsv | tail -1 | cut -f2); case "$s" in MATCH|RELOCATION_MATCH) continue;; esac; bash $W/s7/combo.sh $a >> $W/s7/cbsweep2.tsv 2>&1; done
echo DONE >> $W/s7/cbsweep2.tsv
