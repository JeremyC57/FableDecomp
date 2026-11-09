#!/bin/bash
src=$(readlink -f $1)
# inl.sh <src.cpp> <tag> [flags]: inline-decision trace -> tools/inl/<tag>.json, <tag>.txt
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
A=${A:-$S/w22/agent883}; D=$A/tools/inl
export C2PRIO_RUNS=$D/runs FABLE_HDR_OVERLAY=${HDR:-$A/hdr}
t=$2; shift 2
timeout 1500 python3 $D/inl_host.py $src $D/$t.json "$@" > $D/$t.out 2>&1; echo "rc $?"; tail -2 $D/$t.out
python3 $D/an.py $D/$t.json ${FUNC:-?ConstructLogbookListPC@} > $D/$t.txt; head -1 $D/$t.txt
