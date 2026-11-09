#!/bin/bash
# gq.sh <name> [flags]: grade probe/<name>.cpp (q.py, own row only), save obj to tmp/<name>.obj, print cmpx summary + segs count
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent883
n=$1; shift
bash $A/gp.sh $n "$@" | head -3
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad FABLE_OBJDUMP=objdump
cp $S/wine/q/006096e0/006096e0.obj $A/tmp/$n.obj
L='?ConstructLogbookListPC@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z'
python3 $A/tools/cmpx.py $A/tmp/$n.obj 006096e0 "$L"
python3 $A/tools/cmpx.py $A/tmp/$n.obj 006096e0 "$L" --segs --mask frame | tail -1
