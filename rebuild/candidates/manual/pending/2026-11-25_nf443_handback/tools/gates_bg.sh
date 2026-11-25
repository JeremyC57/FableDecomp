#!/usr/bin/env bash
# gates_bg.sh <batch>: objgate + pk + link (definers = the renames' targets, objects in work/g/<t>_D) for one batch after runpair
W=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF443; b=$1; cd $W
python3 tools/gate438.py $b $b > work/og_$b.out 2>&1
python3 tools/pkgate438.py $b $b > work/pk_$b.out 2>&1
timeout 900 python3 tools/linkgate438.py $b $b $(cut -f4 batches/$b/renames.tsv | sort -u) > work/lk_$b.out 2>&1
echo done > work/gates_$b.done
