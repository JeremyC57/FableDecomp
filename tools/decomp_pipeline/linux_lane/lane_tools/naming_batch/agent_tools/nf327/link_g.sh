#!/bin/bash
# link_g.sh <tag> <changed list> <control list> <watch regex> <donefile>: nf.py link of the HEAD set (H_ changed + HEAD controls)
# v the new set (new changed + the same controls), on its own `timeout 900` line
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
A=$S/w22/agentNF327; W=$A/work; cd $W; h=(); n=(); c=()
for a in $(cat $2); do h+=($W/obj/H_$a.obj); n+=($W/obj/$a.obj); done
for a in $(cat $3); do grep -qx $a $2 && continue; [ -f $W/hobj/$a.obj ] && c+=($W/hobj/$a.obj); done
timeout 900 python3 $A/tools/nf.py link $1 "$4" "${h[@]}" "${c[@]}" -- "${n[@]}" "${c[@]}"
echo done > $5
