#!/bin/bash
# dd.sh <addr>: differing lines (address-stripped, call targets ignored) between retail ref and tmp/m.txt
cut -c1-9,30- $W/ref/$1/retail_dis.txt | sed 's/  */ /' > $W/tmp/r.txt; sed 's/  */ /' $W/tmp/m.txt > $W/tmp/mm.txt
paste -d'|' $W/tmp/r.txt $W/tmp/mm.txt | awk -F'|' '{a=$1;b=$2; sub(/^[0-9a-f]+ /,"",a); sub(/^[0-9a-f]+ /,"",b); if(a ~ /^call/ && b ~ /^call/) next; if(a ~ /^j/ && b ~ /^j/ && substr(a,1,3)==substr(b,1,3)) next; if(a!=b) printf "%-46s | %s\n",$1,$2}'
