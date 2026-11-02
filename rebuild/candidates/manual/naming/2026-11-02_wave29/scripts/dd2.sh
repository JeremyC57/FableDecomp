#!/bin/bash
# dd2.sh <ov> <addr> <len> <leafgrep>: normalized instruction diff retail vs built leaf
S=${S:?}; W=$S/naming/w29; : "${W:?}"; ov=$1; a=$2; n=$3; lg=$4
bash $W/scripts/ddiff.sh $ov $a $n > /dev/null || exit 1
d=$W/pw/dd_$a
awk -v lg="$lg" 'index($0,lg)&&/>:$/{f=1;next} f&&/>:$/{f=0} f' $d/a.dis | grep -viE "dir32|disp32|rel32|secrel" | grep -E "^\s+[0-9a-f]+:" | awk -F'\t' '{print $NF}' | sed -E 's/0x[0-9a-f]+|[0-9a-f]+ <[^>]*>//g' > $d/b.txt
grep -E "^\s+[0-9a-f]+:" $d/r.dis | awk -F'\t' '{print $NF}' | sed -E 's/0x[0-9a-f]+|[0-9a-f]+ <[^>]*>//g' > $d/r.txt
diff $d/r.txt <(head -$(wc -l < $d/r.txt) $d/b.txt)
