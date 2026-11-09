#!/bin/bash
src=$(readlink -f $1)
# wi.sh <src> <tag> 'id=prio,...' ['name=prio'] : what-if compile + cmpx summary
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad FABLE_OBJDUMP=objdump; cd /home/user/FableDecomp
A=${A:-$S/w22/agent882}; D=$A/tools/wi
export C2PRIO_RUNS=$D/runs FABLE_HDR_OVERLAY=${HDR:-$A/hdr}
t=$2; shift 2
timeout 1500 python3 $D/wi.py $src $t "$@" 2>&1 | tail -3
L=${LEAF:-'?ConstructQuestListPC@CTCInventoryQuests@@QAEXPAVCComponent@NUISystem@@ABV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@PAVCList@3@W4QuestScreenType@@2@Z'}
python3 $A/tools/cmpx.py $D/$t.obj ${ADDR:-0061e6d0} "$L"
python3 $A/tools/cmpx.py $D/$t.obj ${ADDR:-0061e6d0} "$L" --segs --mask frame | tail -30
