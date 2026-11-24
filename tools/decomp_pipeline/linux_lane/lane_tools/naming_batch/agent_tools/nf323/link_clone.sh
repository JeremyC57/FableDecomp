#!/bin/bash
# link_clone.sh <tag> <watch regex> <side-dirs "h1 h2.." > <new-dirs> <ctl dir>: /FORCE link (nf.py link) of every HEAD Clone object +
# the control objects v every new Clone object + the same controls -> evidence/link_<tag>.txt
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
A=$S/w22/agentNF323; W=$A/work; cd $W
h=(); n=(); c=()
for d in $3; do h+=($(find $W/$d -name '*.obj' ! -name '*.t.obj')); done
for d in $4; do n+=($(find $W/$d -name '*.obj' ! -name '*.t.obj')); done
c=(); for d in $5; do c+=($(find $W/$d -name "*.obj" ! -name "*.t.obj")); done
timeout 900 python3 $A/tools/nf.py link $1 "$2" "${h[@]}" "${c[@]}" -- "${n[@]}" "${c[@]}"
echo done > $W/lk_$1.done
