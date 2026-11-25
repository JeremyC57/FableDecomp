#!/usr/bin/env bash
# mkdefs.sh <addr...>: compile HEAD definer rows (repo sources, vland --recheck-landed at rsroot) into work/g/<a>_D/<a>.obj
set -u; TAG=${TAG:-def}
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; W=$S/w22/agentNF436
source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP; LT=tools/decomp_pipeline/linux_lane/lane_tools
cd $W/work/rsroot; rm -rf $W/work/p_$TAG $W/work/wd_$TAG; mkdir -p $W/work/p_$TAG
python3 $LT/recheck.py $W/work/p_$TAG "$@" > /dev/null 2>&1
timeout 3000 env FABLE_ROOT=$W/work/rsroot LANDVERIFY_WORK=$W/work/wd_$TAG python3 -u $LT/vland.py $W/work/p_$TAG/payload.json $W/work/p_$TAG/oracle.tsv --recheck-landed > $W/work/vland_$TAG.log 2>&1
for a in "$@"; do o=$(find $W/work/wd_$TAG -name "$a.obj" | head -1); [ -n "$o" ] && { mkdir -p $W/work/g/${a}_D; cp $o $W/work/g/${a}_D/; } || echo "NOOBJ $a"; done
grep -E '^[0-9a-f]{8} +[A-Z]' $W/work/vland_$TAG.log | grep -v WARN
