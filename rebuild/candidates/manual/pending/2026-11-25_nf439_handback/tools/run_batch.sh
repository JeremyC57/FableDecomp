#!/usr/bin/env bash
# run_batch.sh <batch>: runpair (vland HEAD/new one path) then gates (objgate, pk, link) for one batch
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; W=$S/w22/agentNF439; b=$1
source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
cd $W; BATCH=$b bash tools/runpair.sh $b $(cat batches/$b/rows.txt)
python3 tools/gate423.py $b $b > work/og_$b.out 2>&1
python3 tools/pkgate423.py $b $b > work/pk_$b.out 2>&1
echo done > work/run_$b.done
