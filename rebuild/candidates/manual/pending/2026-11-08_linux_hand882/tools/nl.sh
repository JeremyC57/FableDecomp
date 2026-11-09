#!/bin/bash
src=$(readlink -f $1)
# nl.sh <src.cpp> <tag> [flags] : c2 tuple count per function (the 10000 regime-1 gate at c2.dll 0x1077BE49); HDR=<overlay>
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
A=${A:-$S/w22/agent882}
export C2PRIO_RUNS=$A/tools/c2/runs FABLE_HDR_OVERLAY=${HDR:-$A/hdr}
D=$A/tools/c2; t=$2; shift 2
WALKMIN=${WALKMIN:-99999} CNT_JSON=$D/$t.json timeout 1500 python3 $D/cnt_host.py $src "$@" > $D/$t.out 2>&1
echo "rc $?"; grep -E "count|ERROR" $D/$t.out | sort -k2 -n -r | head -5
