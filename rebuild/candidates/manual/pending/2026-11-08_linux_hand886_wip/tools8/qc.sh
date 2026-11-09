#!/bin/bash
# qc.sh <src.cpp> <tag>: plain retail-flag compile via q.py (~16 s) -> t8/o/<tag>.obj; prints grade line + m3 + cross-jump diff count
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent886
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad FABLE_OBJDUMP=objdump FABLE_HDR_OVERLAY=${HDR:-$A/hdr}
f=$(readlink -f $1); t=$2; cd /home/user/FableDecomp
L=$(grep -o "// Leaf: .*" $f | sed 's|// Leaf: ||'); FL=$(grep -o "^// Flags: .*" $f | sed "s|// Flags: ||")
timeout 600 python3 $S/tools/q.py 0041e6d3 $f "$L" ${FL:+--flags $FL} > $A/t8/o/$t.txt 2>&1
head -1 $A/t8/o/$t.txt
if grep -q SRC_FAIL $A/t8/o/$t.txt; then grep -E "error C|fatal" $A/t8/o/$t.txt | sed 's|.*0041e6d3.cpp||' | sort -u | head; exit 1; fi
cp $S/wine/q/0041e6d3/0041e6d3.obj $A/t8/o/$t.obj
python3 $A/t7/m3.py $A/t8/o/$t.obj
python3 $A/t8/xj.py $A/t8/o/$t.obj > $A/t8/o/$t.xj; echo "xj: $(grep -c '^   ' $A/t8/o/$t.xj) differing cross-jumps, $(tail -1 $A/t8/o/$t.xj)"
