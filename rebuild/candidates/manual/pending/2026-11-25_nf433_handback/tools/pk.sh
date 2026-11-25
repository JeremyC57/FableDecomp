#!/usr/bin/env bash
# pk.sh <tag> <addr...>: pk_all on the HEAD and new object of each row; prints per row the non-EQ / reached copies of each side.
source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP; export FABLE_ROOT=/home/user/FableDecomp
W=$S/w22/agentNF433; tag=$1; shift; cd /home/user/FableDecomp
for a in "$@"; do for k in h n; do o=$(ls $W/work/wd_$tag$k/*/*/$a/$a.obj 2>/dev/null | head -1); [ -z "$o" ] && { echo "$a $k NOOBJ"; continue; }
  leaf=$(grep -oP '^// Leaf: \K\S+' $W/work/wd_$tag$k/*/*/$a/$a.cpp 2>/dev/null | head -1)
  python3 -I -B tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py $o $a ${leaf:+--leaf=$leaf} --tsv 2>&1 > $W/work/pk_${tag}${k}_$a.tsv
  printf "%s %s " $a $k; grep -v "^#" $W/work/pk_${tag}${k}_$a.tsv | awk -F'\t' '$1!="EQ" && $1!="SELF" {printf "%s[leaf=%s,reach=%s] %s; ", $1, $8, $9, $10}'; echo
done; done
