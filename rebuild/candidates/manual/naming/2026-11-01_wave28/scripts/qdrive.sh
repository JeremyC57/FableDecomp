#!/bin/bash
# qdrive.sh <overlay> <from> <to>: run fam_q.py for queue targets [from,to) of runs/qtargets2.tsv into the overlay
S=${S:?}; W=$S/naming/w28; ov=$1; i=0
cd /home/user/FableDecomp
while IFS=$'\t' read -r t fin; do
  if [ $i -ge $2 ] && [ $i -lt $3 ]; then
    python3 $W/scripts/spellu.py $t "$fin" $W/runs/q/q_$t.rows.txt > /dev/null
    QTAG=nw19_q$t timeout 900 python3 $W/scripts/fam_q.py $ov $t "$fin" $W/runs/q/q_$t.rows.txt q$t > $W/logs/q_${ov}_$t.log 2>&1
    echo "$i $t $(grep -c ^edited $W/logs/q_${ov}_$t.log)/$(wc -l < $W/runs/q/q_$t.rows.txt)"
  fi
  i=$((i+1))
done < $W/runs/qtargets2.tsv
