#!/bin/bash
# qdrive17.sh <overlay> <targets.tsv> <from> <to>: run fam_q.py / fam_q17.py (third column) for targets [from,to) into the overlay
S=${S:?}; W=$S/naming/w17; ov=$1; tf=$2; i=0
cd /home/user/FableDecomp
while IFS=$'\t' read -r t fin scr; do
  if [ $i -ge $3 ] && [ $i -lt $4 ]; then
    python3 $W/scripts/spellu.py $t "$fin" $W/runs/q/q_$t.rows.txt > /dev/null
    QTAG=nw17_q$t timeout 900 python3 $W/scripts/$scr $ov $t "$fin" $W/runs/q/q_$t.rows.txt q$t > $W/logs/q_${ov}_$t.log 2>&1
    echo "$i $t $(grep -c ^edited $W/logs/q_${ov}_$t.log)/$(wc -l < $W/runs/q/q_$t.rows.txt)"
  fi
  i=$((i+1))
done < $tf
