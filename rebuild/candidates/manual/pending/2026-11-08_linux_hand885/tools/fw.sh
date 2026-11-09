#!/bin/bash
# fw.sh <src.cpp> <tag> [forcereg]: c2prio7 trace (+ optional forced decisions) with /FAs listing -> s7/fw/<tag>.{json,obj,asm,trace}
# then cdiff2 (first colouring-order divergences v retail) and first.py / m3x.
source /tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent885/tools/pk/env.sh
src=$(readlink -f $1); tag=$2; fr=$3; D=$A/s7/fw; mkdir -p $D
cd /home/user/FableDecomp
FL="$(grep -o '^// Flags: .*' $src | sed 's|// Flags: ||') /FAs /Fa$(winepath -w $D/$tag.asm)"
timeout 1500 python3 $A/tools/pk/c2prio7.py $src --func RunCutsceneMacro --trace --flags "$FL" --json $D/$tag.json --obj $D/$tag.obj ${fr:+--forcereg $fr} > $D/$tag.trace 2>&1 || { echo "$tag FAIL"; tail -5 $D/$tag.trace; exit 1; }
cd $A
python3 tools/pk/cdiff2.py $D/$tag.obj $D/$tag.asm $D/$tag.json 233 --n 25 > $D/$tag.cdiff
echo "$tag $(python3 tools/pk/m3x.py $D/$tag.obj) | first $(python3 tools/pk/first.py $D/$tag.obj --noframe --n 2 | tr '\n' ';' | cut -c1-160)"
head -12 $D/$tag.cdiff
gzip -f $D/$tag.json $D/$tag.asm
