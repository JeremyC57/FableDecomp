#!/bin/bash
# regrade.sh <n> <filelist>: recheck.py payload + vland --recheck-landed for one batch; log to $S/mig9/logs/recheck<n>.log
export S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad
source $S/wine/env.sh; export FABLE_ROOT=$S/mig9/repo; cd $S/mig9/repo
n=$1; D=$S/mig9/work/rc$n; mkdir -p $D
L=tools/decomp_pipeline/linux_lane/lane_tools
python3 $L/recheck.py $D $(cat $2) > $D/recheck.out 2>&1
export LANDVERIFY_WORK=$S/mig9/work/lv$n
timeout 7000 python3 $L/vland.py $D/payload.json $D/oracle.tsv --recheck-landed > $S/mig9/logs/recheck$n.log 2>&1
echo "exit $?" >> $S/mig9/logs/recheck$n.log
rm -rf $S/mig9/work/lv$n
