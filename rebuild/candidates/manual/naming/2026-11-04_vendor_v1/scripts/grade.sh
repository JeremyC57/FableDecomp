#!/bin/bash
# grade.sh <n> : vland --recheck-landed (read-only) on pay_<n> with the overlay headers -> logs/recheck_<n>.log
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh >/dev/null 2>&1
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
cd /home/user/FableDecomp; W=$S/naming/vendor/w1; : "${W:?}"; N=$1
rm -rf $W/vw_$N
FABLE_HDR_OVERLAY=$W/overlay/rebuild/include LANDVERIFY_WORK=$W/vw_$N timeout 2400 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $W/pay_$N/payload.json $W/pay_$N/oracle.tsv --recheck-landed > $W/logs/recheck_$N.log 2>&1
echo done $N
