#!/usr/bin/env bash
# prekeep.sh <batch>: precheck (prefam.sh), then drop rows whose NEW source fails (they stay HOLD in their source batch) and run the vland pair
W=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF438; b=$1; B=$W/batches/$b; cd $W
bash tools/prefam.sh $b
fail=$(for a in $(cat $B/rows.txt); do grep -q " ${b}N_$a PASS" work/pre.log || echo $a; done)
for a in $fail; do python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py $W $B/rows/$a.cpp $B/orig/rows/$a.cpp $B/orig/tests/${a}_test.cpp >/dev/null; echo $a >> work/${b}_prefail.txt; done
for f in rows.txt renames.tsv; do grep -v -F -f <(printf "%s\n" $fail dummy_xx) $B/$f > work/x_$b; cp work/x_$b $B/$f; done
BATCH=$b bash tools/runpair.sh $b $(cat $B/rows.txt)
bash tools/gates_bg.sh $b
