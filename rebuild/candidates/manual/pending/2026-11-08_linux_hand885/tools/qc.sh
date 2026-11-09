#!/bin/bash
# qc.sh <src.cpp> <tag> [refobj]: plain compile (mc.py, no probe) -> s7/qc_<tag>.obj; prints bytes, first register-level
# differences v retail (first.py --noframe --n 3), m3x, frame size, and whether the function's code equals refobj's
# (default s7/ref_a3.obj; "SAME" = byte-identical function, "SAMEMASK" = identical with [ebp+-x] masked).
source /tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent885/tools/pk/env.sh
src=$(readlink -f $1); tag=$2; ref=${3:-$A/s7/ref_a3.obj}; D=$A/s7/qcw/$tag
cd /home/user/FableDecomp
t0=$(date +%s)
timeout 600 python3 $A/tools/mc.py $src $D > $A/s7/qc_$tag.log 2>&1 || { echo "$tag FAIL"; grep -E "error C|fatal" $D/../../qc_$tag.log $A/s7/qc_$tag.log | head -5; exit 1; }
mv $D/m.obj $A/s7/qc_$tag.obj; rm -rf $D
cd $A
f=$(python3 tools/pk/first.py s7/qc_$tag.obj --noframe --n 3 | tr '\n' ';')
m=$(python3 tools/pk/m3x.py s7/qc_$tag.obj)
s=$(python3 tools/pk/same.py s7/qc_$tag.obj $ref)
line="$tag	$(echo $m | cut -d' ' -f1)	$s	$(echo $m | sed 's/.*exact /exact /')	first $(echo $f | cut -c1-200)	$(( $(date +%s)-t0 ))s"
echo "$line"; echo "$(date +%H:%M) $line" >> $A/s7/LEADERBOARD.tsv
