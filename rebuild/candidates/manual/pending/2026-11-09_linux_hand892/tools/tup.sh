#!/bin/bash
# tup.sh <src.cpp> <out.json> '<json cfg {"target": substr}>' : dump the raw IL tuple runs the region finder walks
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
A=$S/w22/agent892
export FABLE_HDR_OVERLAY=${FABLE_HDR_OVERLAY:-$A/hdr} C2PRIO_RUNS=$A/wk/c2runs
timeout 600 python3 $A/tools/sched/host.py $A/tools/sched/t14.py "$1" "$2" "$3"
