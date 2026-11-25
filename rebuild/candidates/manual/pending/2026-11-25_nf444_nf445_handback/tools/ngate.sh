#!/bin/bash
# ngate.sh: name_gate on every batch row, HEAD source (empty overlay) and new source (overlay, /Ob1); writes evidence/name_gate_{head,new}.txt
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; export S
source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
W=$S/w22/agentNF445; B=$W/batches/nf444_hdr; cd /home/user/FableDecomp
: > $B/evidence/name_gate_head.txt; : > $B/evidence/name_gate.txt
for a in $(cat $B/rows.txt); do
  s=$(ls rebuild/src/compiled/00/c*/*_$a.cpp); hl=$(awk -F'\t' -v a=$a '$1""==a""{print $2}' $B/RESULTS.tsv); nl=$(awk -F'\t' -v a=$a '$1""==a""{print $3}' $B/RESULTS.tsv | head -1)
  hl=$(awk -F'\t' -v a=$a 'NR>1 && $1""==a""{print $3}' $B/RESULTS.tsv); nl=$(awk -F'\t' -v a=$a 'NR>1 && $1""==a""{print $4}' $B/RESULTS.tsv)
  echo "## $a $hl" >> $B/evidence/name_gate_head.txt
  FABLE_HDR_OVERLAY=$W/empty_overlay timeout 300 python3 $S/tools/name_gate.py $a $s "$hl" 2>&1 | grep -E "^(FAIL|NAME_GATE)" | cut -c1-230 >> $B/evidence/name_gate_head.txt
  echo "## $a $nl" >> $B/evidence/name_gate.txt
  FABLE_HDR_OVERLAY=$W/hdr timeout 300 python3 $S/tools/name_gate.py $a $B/rows/$a.cpp "$nl" --flags /Ob1 2>&1 | grep -E "^(FAIL|NAME_GATE)" | cut -c1-230 >> $B/evidence/name_gate.txt
done
echo DONE >> $B/evidence/name_gate.txt
