#!/bin/bash
# fv.sh <src.cpp> <force.json> [flags]: forced-decision build + masked diff summary (out: <src>.f.obj/.f.md/.f.json)
cd /home/user/FableDecomp; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1
T=$S/w22/auth10/tools; src=$1; fj=$2; shift 2; b=${src%.cpp}
export FABLE_HDR_OVERLAY=${FABLE_HDR_OVERLAY:-$S/w22/auth10/008e18d0/hdr}
OBJ_OUT=$b.f.obj timeout 1500 python3 $T/force_host.py $src $b.f.json $fj "$@" | tail -1 | grep -v '^events'
python3 $T/mdiff.py ${ROW:-008e18d0} $b.f.obj "${LEAF:-?UpdateLightningBeams@CModeThunderLightningStorm@NCreatureMode@@AAEXXZ}" > $b.f.md
python3 -c "
import json;E=json.load(open('$b.f.json')); print('forced', sum(1 for e in E if e[0]=='d' and 'forced' in e[1]))" 
echo "$(basename $src) $(tail -1 $b.f.md) hunks $(grep -c '^@@' $b.f.md)"
