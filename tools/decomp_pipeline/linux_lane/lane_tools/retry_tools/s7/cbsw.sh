#!/bin/bash
W=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/retry10
for a in $(cat $W/s7/cb_cands.txt); do bash $W/s7/combo.sh $a >> $W/s7/cbsweep.tsv 2>&1; done
echo DONE >> $W/s7/cbsweep.tsv
