#!/usr/bin/env bash
# qall.sh <dir> <addr...>: q.py every row (leaf from its // Leaf: line); prints one status line each (+ diff on request QV=1)
source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP; export FABLE_HDR_OVERLAY=$S/w22/agentNF433/hdr
d=$1; shift; cd /home/user/FableDecomp
for a in "$@"; do leaf=$(grep -oP '^// Leaf: \K\S+' $d/rows/$a.cpp); out=$(python3 tools/decomp_pipeline/linux_lane/lane_tools/q.py $a $d/rows/$a.cpp "$leaf" 2>&1); echo "$out" | head -1; if [ -n "$QV" ] && ! echo "$out" | head -1 | grep -q "MATCH flags"; then echo "$out" | sed -n '2,60p'; fi; done
