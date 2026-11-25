#!/usr/bin/env bash
# elig.sh <addr...>: lock owner, reserved hits, newest claim account, T3 queue lines (col2) per address
cd /home/user/FableDecomp; M=rebuild/candidates/manual
for a in "$@"; do A=$(echo $a | tr a-f A-F); own=$(cat $S/w22/nflocks/$a/owner 2>/dev/null || echo none)
 res=$(grep -il "$a" $M/pending/*/RESERVED.tsv 2>/dev/null | wc -l)
 cl=$(grep -i "$a" $M/COORDINATION_CLAIMS.tsv | tail -1 | cut -f2)
 t3=$(awk -F'\t' -v x="0x$A" '$2""==x""{print $1}' $M/naming/T3_PORT_QUEUE.tsv | sort -u | tr '\n' ,)
 t3c=$(awk -F'\t' -v x="$A" 'index(toupper($3),x)>0{print $1}' $M/naming/T3_PORT_QUEUE.tsv | sort -u | tr '\n' ,)
 echo -e "$a\t$own\tres=$res\tclaim=$cl\tt3=$t3\tt3c3=$t3c"; done
