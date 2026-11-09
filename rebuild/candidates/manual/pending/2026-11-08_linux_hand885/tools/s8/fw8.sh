#!/bin/bash
# fw8.sh <src.cpp> <tag> [snapk] [extra c2prio8 args...]: c2prio8 trace with const-piece snapshots -> s8/fw/<tag>.{json,obj,asm,trace}
source /tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent885/tools/pk/env.sh
src=$(readlink -f $1); tag=$2; sk=${3:-0}; shift 3; D=$A/s8/fw; mkdir -p $D
cd /home/user/FableDecomp
FL="$(grep -o '^// Flags: .*' $src | sed 's|// Flags: ||') /FAs /Fa$(winepath -w $D/$tag.asm)"
timeout 1800 python3 $A/s8/tools/c2prio8.py $src --func RunCutsceneMacro --trace --flags "$FL" --json $D/$tag.json --obj $D/$tag.obj --snapk $sk "$@" > $D/$tag.trace 2>&1 || { echo "$tag FAIL"; tail -5 $D/$tag.trace; exit 1; }
cd $A
python3 tools/pk/cdiff2.py $D/$tag.obj $D/$tag.asm $D/$tag.json 235 --n 25 > $D/$tag.cdiff
echo "$tag $(python3 tools/pk/m3x.py $D/$tag.obj) | first $(python3 tools/pk/first.py $D/$tag.obj --noframe --n 2 | tr '\n' ';' | cut -c1-160)"
head -12 $D/$tag.cdiff
