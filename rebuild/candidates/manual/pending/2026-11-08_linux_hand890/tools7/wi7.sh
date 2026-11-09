#!/bin/bash
# wi7.sh <src> <tag> <force> <forceif> [watch]: what-if run + dd.py summary (cwd = work/s7)
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
A=$S/w22/agent890; W=$A/work/s7; cd $W
export FABLE_HDR_OVERLAY=$A/hdr C2PRIO_RUNS=$W/runs
timeout 900 python3 $W/tools/wi7.py "$(readlink -f $1)" $2 "$3" "$4" "$5" > $W/wi/$2.out 2>&1
grep -c chosen $W/wi/$2.out; grep forced $W/wi/$2.out | head
[ -f $W/wi/$2.obj ] && timeout 120 python3 $A/work/dd.py 0056ad40 $W/wi/$2.obj --leaf '?Activate@CTCBoastUI@@' --ctx 0 | head -2
