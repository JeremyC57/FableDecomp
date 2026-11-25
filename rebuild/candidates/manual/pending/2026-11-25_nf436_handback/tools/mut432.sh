#!/usr/bin/env bash
# mut432.sh <batch> <newroot-name> <mutator.py>: mutation control. The mutator (python, reads/writes the new payload's sources) mutates
# each row; vland runs at the batch's LANDVERIFY_WORK path; the tests must FAIL. Log: work/vland_<b>m.log
set -u
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; W=$S/w22/agentNF436; b=$1; R=$W/work/$2
source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP
LT=tools/decomp_pipeline/linux_lane/lane_tools
mkdir -p $W/work/p_${b}m; python3 $3 $W/work/p_${b}n/payload.json $W/work/p_${b}m/payload.json; cp $W/work/p_${b}n/oracle.tsv $W/work/p_${b}m/
cd $R; rm -rf $W/work/wd_$b
timeout 3600 env FABLE_ROOT=$R LANDVERIFY_WORK=$W/work/wd_$b python3 -u $LT/vland.py $W/work/p_${b}m/payload.json $W/work/p_${b}m/oracle.tsv --recheck-landed > $W/work/vland_${b}m.log 2>&1
rm -rf $W/work/wd_${b}m; mv $W/work/wd_$b $W/work/wd_${b}m
grep -E '^[0-9a-f]{8} +[A-Z_]+ +(PASS|FAIL)' $W/work/vland_${b}m.log
