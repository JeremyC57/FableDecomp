#!/usr/bin/env bash
# ngate428.sh <batch>: name_gate.py on every row text (its new leaf) with its test -> <batch>/evidence/name_gate.txt (last line per row)
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF440; B=$A/runs/$1
cd /home/user/FableDecomp; NG=tools/decomp_pipeline/linux_lane/lane_tools/name_gate.py
: > $B/evidence/name_gate.txt
while IFS=$'\t' read a mem t old new d form; do
  python3 $NG $a $B/rows/$a.cpp "$new" --test $B/tests/${a}_test.cpp > $B/evidence/ng_$a.log 2>&1
  echo "$a $(tail -1 $B/evidence/ng_$a.log)" >> $B/evidence/name_gate.txt
done < $B/PLAN.tsv
echo NG_DONE >> $B/evidence/name_gate.txt
