#!/usr/bin/env bash
# ngate.sh <batch> [root]: name_gate.py on every row + test of the batch (leaf = RESULTS new). Output work/ng_<b>.tsv
set -u
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; A=$S/w22/agentNF430; b=$1; R=${2:-$A/work/rsnew}; B=$A/batches/$b
source $S/wine/env.sh; export S; unset TMPDIR TMP
export NG_TABLE=$R/rebuild/oracles/reloc_bindings.tsv; cd $R; out=$A/work/ng_$b.tsv; : > $out
tail -n +2 $B/RESULTS.tsv | while IFS=$'\t' read -r a v o n r; do
  res=$(timeout 600 python3 $A/tools/name_gate430.py $a $B/rows/$a.cpp "$n" --test $B/tests/${a}_test.cpp 2>&1)
  verdict=$(echo "$res" | grep -oE "^(PASS|FAIL)[^ ]*" | tail -1)
  fails=$(echo "$res" | grep -E "^\s*FAIL" | sed 's/^\s*//' | cut -c1-160 | paste -sd'|')
  echo -e "$a\t${verdict:-?}\t$fails" >> $out
done
echo NG_DONE $b
