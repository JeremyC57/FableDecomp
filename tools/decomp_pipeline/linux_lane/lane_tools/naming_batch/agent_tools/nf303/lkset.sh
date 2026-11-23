#!/bin/bash
# lkset.sh <tag> <watch-regex> <binders,comma> <owners,comma>: lk.sh with HEAD binders + HEAD owners v new binders + HEAD owners
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF303; O=$A/work/obj
h=(); n=(); for b in ${3//,/ }; do h+=($O/H_$b.obj); n+=($O/N_$b.obj); done; for o in ${4//,/ }; do h+=($O/H_$o.obj); n+=($O/H_$o.obj); done
bash $A/tools/lk.sh $1 "$2" "${h[@]}" -- "${n[@]}"
