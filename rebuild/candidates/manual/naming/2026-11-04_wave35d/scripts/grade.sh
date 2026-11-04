#!/bin/bash
# grade.sh <tag> [addr...]: build the payload from the overlay and grade it read-only (vland --recheck-landed)
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh >/dev/null 2>&1
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
cd /home/user/FableDecomp; W=$S/wave35d; T=$1; shift
rm -rf $W/rk_$T $W/vw_$T; python3 $W/scripts/build.py $W/rk_$T "$@"
LANDVERIFY_WORK=$W/vw_$T timeout 2400 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $W/rk_$T/payload.json $W/rk_$T/oracle.tsv --recheck-landed > $W/logs/recheck_$T.log 2>&1
python3 $W/scripts/passlist.py $T $W/logs/recheck_$T.log > $W/rk_$T/pass.tsv; cut -f1 $W/rk_$T/pass.tsv | sort | uniq -c
