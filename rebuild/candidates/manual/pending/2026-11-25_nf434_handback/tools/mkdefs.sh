#!/usr/bin/env bash
# mkdefs.sh <addr...>: compile HEAD definer rows (repo sources, vland --recheck-landed at rsroot) into work/g/<a>_D/<a>.obj
set -u
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; W=$S/w22/agentNF434
source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP; LT=tools/decomp_pipeline/linux_lane/lane_tools
cd $W/work/rsroot; rm -rf $W/work/p_def $W/work/wd_def; mkdir -p $W/work/p_def
python3 $LT/recheck.py $W/work/p_def "$@" > /dev/null 2>&1
timeout 3000 env FABLE_ROOT=$W/work/rsroot LANDVERIFY_WORK=$W/work/wd_def python3 -u $LT/vland.py $W/work/p_def/payload.json $W/work/p_def/oracle.tsv --recheck-landed > $W/work/vland_def.log 2>&1
for a in "$@"; do o=$(find $W/work/wd_def -name "$a.obj" | head -1); [ -n "$o" ] && { mkdir -p $W/work/g/${a}_D; cp $o $W/work/g/${a}_D/; } || echo "NOOBJ $a"; done
grep -E '^[0-9a-f]{8} +[A-Z]' $W/work/vland_def.log | grep -v WARN
