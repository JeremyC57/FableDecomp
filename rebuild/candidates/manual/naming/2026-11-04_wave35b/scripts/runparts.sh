#!/bin/bash
# runparts.sh <dir> : run vland --recheck-landed over <dir>/rk_part_* (payload.json/oracle.tsv), at most 3 at once
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
cd /home/user/FableDecomp
D=$1
for r in $D/rk_part_*/; do
  b=$(basename $r)
  while [ $(jobs -r | wc -l) -ge 3 ]; do sleep 5; done
  ( LANDVERIFY_WORK=$r/work FABLE_HDR_OVERLAY=${HDROV:-} timeout 2400 python3 -u ${VLAND:-tools/decomp_pipeline/linux_lane/lane_tools/vland.py} $r/payload.json $r/oracle.tsv --recheck-landed > $D/$b.log 2>&1 ) &
done
wait
cat $D/rk_part_*.log > $D/recheck.log
echo done
