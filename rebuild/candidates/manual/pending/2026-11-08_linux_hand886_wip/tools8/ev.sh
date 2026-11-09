#!/bin/bash
# ev.sh <src.cpp> <tag>: hooked compile (phook) -> t8/fr/<tag>.{json,obj,asm}; then m3 + fmap + packer fitness.
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent886
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad FABLE_OBJDUMP=objdump
src=$(readlink -f $1); t=$2
g++ -fsyntax-only -E -P -x c++ -I$A/hdr -I/home/user/FableDecomp/rebuild/include $src -o /dev/null 2>$A/t8/fr/$t.pp || { echo "PP FAIL"; head -5 $A/t8/fr/$t.pp; exit 1; }
HOOK=phook.py $A/t8/fr/run.sh $src $t > $A/t8/fr/$t.log 2>&1 || { echo "COMPILE FAIL"; tail -20 $A/t8/fr/$t.log; exit 1; }
grep -q "rc 0" $A/t8/fr/$t.log || { echo "CL FAIL"; grep -E "error|rc" $A/t8/fr/$t.log | head; exit 1; }
python3 $A/t7/m3.py $A/t8/fr/$t.obj
python3 $A/t7/fmap.py $A/t8/fr/$t.obj > $A/t8/fr/$t.fmap 2>&1; tail -1 $A/t8/fr/$t.fmap
python3 $A/t8/fr/fit.py $t > $A/t8/fr/$t.fit 2>&1; head -1 $A/t8/fr/$t.fit
rm -f $A/t8/fr/$t.asm
