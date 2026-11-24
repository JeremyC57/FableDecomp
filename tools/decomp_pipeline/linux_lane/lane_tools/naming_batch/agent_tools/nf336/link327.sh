#!/bin/bash
# link327.sh <tag> "<head dirs>" "<new dirs>" "<control dirs>": /FORCE link (nf.py link, timeout 900) of every HEAD object + controls v every new
# object + the same controls -> evidence/link_<tag>.txt, work/lk_<tag>.done (NF327, from NF323's link_clone.sh)
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP   # NF332
A=$S/w22/agentNF336; W=$A/work; cd $W
h=(); n=(); c=()
for d in $2; do h+=($(find $W/$d -name '*.obj' ! -name '*.t.obj' ! -name '*_test*')); done
for d in $3; do n+=($(find $W/$d -name '*.obj' ! -name '*.t.obj' ! -name '*_test*')); done
for d in $4; do c+=($(find $W/$d -name '*.obj' ! -name '*.t.obj' ! -name '*_test*')); done
timeout 900 python3 $A/tools/nf.py link $1 "." "${h[@]}" "${c[@]}" -- "${n[@]}" "${c[@]}"
echo done > $W/lk_$1.done
