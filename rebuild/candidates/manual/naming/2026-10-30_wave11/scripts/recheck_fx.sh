#!/bin/bash
# recheck.sh <tag> <overlay> <rows.txt> [nparts]: vland --recheck-landed (via vrun.py, pairs recorded) of the rows, from the overlay
S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad; source $S/wine/env.sh; W=$S/naming/w11
tag=$1; ov=$2; rf=$3; np=${4:-2}
cd /home/user/FableDecomp
sort -u $rf | split -n r/$np - $W/runs/rk_$tag.part.
for p in $W/runs/rk_$tag.part.*; do
  k=${p##*.}
  python3 $W/scripts/mkpayload_fx.py $W/runs/rk_${tag}_$k $W/$ov $p > /dev/null
  LANDVERIFY_WORK=$W/lv timeout 7000 python3 $W/scripts/vrun.py $W/runs/rk_${tag}_${k}_payload.json $W/runs/rk_${tag}_${k}_oracle.tsv $W/runs/rk_${tag}_$k.pairs.jsonl > $W/runs/rk_${tag}_$k.log 2>&1 &
done
wait
cat $W/runs/rk_${tag}_*.log > $W/runs/rk_$tag.log; cat $W/runs/rk_${tag}_*.pairs.jsonl > $W/runs/rk_$tag.pairs.jsonl
grep -E "^[0-9a-f]{8} +[A-Z_]+(\(|\[| )" $W/runs/rk_$tag.log | grep -v " WARN " | awk '{print $2, $3}' | sed 's/\[.*\]//' | sort | uniq -c
grep -E "^[0-9a-f]{8} +[A-Z_]+" $W/runs/rk_$tag.log | grep -v " WARN " | grep -vE "MATCH(\[[^]]*\])? +PASS" | head -30
