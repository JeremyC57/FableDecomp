#!/usr/bin/env bash
# ngate432.sh <batch> <table>: name_gate (NG_TABLE copy, FABLE_HDR_OVERLAY=$W/hdr) on every row + test; leaf = the row's // Leaf:. Output batches/<b>/evidence/name_gate.tsv
set -u
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; W=$S/w22/agentNF434; b=$1; B=$W/batches/$b
source $S/wine/env.sh; export S; unset TMPDIR TMP
export NG_TABLE=$2 FABLE_HDR_OVERLAY=${NG_OVERLAY:-$W/hdr}; cd /home/user/FableDecomp; mkdir -p $B/evidence; out=$B/evidence/name_gate.tsv; : > $out
for a in $(cat $B/rows.txt); do
  leaf=$(grep -m1 '^// Leaf:' $B/rows/$a.cpp | awk '{print $3}')
  res=$(timeout 600 python3 $W/tools/name_gate430.py $a $B/rows/$a.cpp "$leaf" --test $B/tests/${a}_test.cpp 2>&1)
  echo "$res" > $W/work/ng_${b}_$a.log
  verdict=$(echo "$res" | grep -oE "^NAME_GATE (PASS|FAIL.*)" | tail -1)
  fails=$(echo "$res" | grep -E "^\s*(FAIL|DEBT)" | sed 's/^\s*//' | cut -c1-200 | paste -sd'|')
  echo -e "$a\t${verdict:-?}\t$fails" >> $out
done
cat $out
