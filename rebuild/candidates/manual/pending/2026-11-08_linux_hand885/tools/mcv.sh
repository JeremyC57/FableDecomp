#!/bin/bash
# compile a variant into probe/mc/<label>/ and summarise its call sites (no grade): mcv.sh <file.cpp> <label>
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
A=$S/w22/agent885; export FABLE_HDR_OVERLAY=${HV:+$HV:}$A/hdr
f=$(realpath $1); lab=$2; d=$A/probe/mc/$lab
cd /home/user/FableDecomp
timeout 400 python3 $A/tools/mc.py $f $d > $A/probe/mc_$lab.log 2>&1 || { cat $A/probe/mc_$lab.log | head -30; exit 1; }
cd $A
CALLS_OBJ=$d/m.obj python3 tools/calls.py --counts --ctx 0 --max 400 > probe/calls_mc_$lab.txt 2>&1
head -1 probe/calls_mc_$lab.txt
python3 tools/sites.py probe/calls_mc_$lab.txt
