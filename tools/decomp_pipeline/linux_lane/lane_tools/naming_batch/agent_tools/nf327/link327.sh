#!/bin/bash
# link327.sh <tag> "<head dirs>" "<new dirs>" "<control dirs>": /FORCE link (nf.py link, timeout 900) of every HEAD object + controls v every new
# object + the same controls -> evidence/link_<tag>.txt, work/lk_<tag>.done (NF327, from NF323's link_clone.sh)
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
A=$S/w22/agentNF327; W=$A/work; cd $W
h=(); n=(); c=()
for d in $2; do h+=($(find $W/$d -name '*.obj' ! -name '*.t.obj' ! -name '*_test*')); done
for d in $3; do n+=($(find $W/$d -name '*.obj' ! -name '*.t.obj' ! -name '*_test*')); done
for d in $4; do c+=($(find $W/$d -name '*.obj' ! -name '*.t.obj' ! -name '*_test*')); done
timeout 900 python3 $A/tools/nf.py link $1 "." "${h[@]}" "${c[@]}" -- "${n[@]}" "${c[@]}"
echo done > $W/lk_$1.done
