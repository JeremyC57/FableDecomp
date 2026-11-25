#!/usr/bin/env bash
# ng.sh <dir> <addr...>: name_gate on row + test (if present); prints the non-OK lines and the verdict
source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP; export FABLE_HDR_OVERLAY=$S/w22/agentNF433/hdr
d=$1; shift; cd /home/user/FableDecomp
for a in "$@"; do leaf=$(grep -oP '^// Leaf: \K\S+' $d/rows/$a.cpp); t=""; [ -f $d/tests/${a}_test.cpp ] && t="--test $d/tests/${a}_test.cpp"
  python3 tools/decomp_pipeline/linux_lane/lane_tools/name_gate.py $a $d/rows/$a.cpp "$leaf" $t > $d/ng_$a.log 2>&1
  echo "$a $(tail -1 $d/ng_$a.log)"; grep -v "^OK \|NAME_GATE\|^$a " $d/ng_$a.log | head -5 | cut -c1-250; done
