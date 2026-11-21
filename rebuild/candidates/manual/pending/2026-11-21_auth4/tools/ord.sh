#!/bin/bash
# ord.sh <tag> <src> [overlay] : dump2 readout + order.py
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; T=$S/w22/auth4/tools; W=$S/w22/auth4/00a4efc0
source $S/wine/env.sh >/dev/null 2>&1; export FABLE_HDR_OVERLAY=${3:-$W/hdr}
( cd $T/sched && python3 host.py $T/il/dump2.py $2 $W/h/$1.il.json '{"flags":["/Gr"]}' >/dev/null 2>&1 )
echo "$1: $(python3 $T/il/order.py $W/h/$1.il.json | head -1)"
