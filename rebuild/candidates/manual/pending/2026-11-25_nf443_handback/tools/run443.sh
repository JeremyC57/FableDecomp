#!/usr/bin/env bash
# run443.sh <batch>: definers (predef) + runpair (vland HEAD/new one path) + objgate, pk, /FORCE link for one batch
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; W=$S/w22/agentNF443; b=$1
source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP; export PYTHONDONTWRITEBYTECODE=1
cd $W
cut -f4 batches/$b/renames.tsv | sort -u > work/${b}_tg.txt
todo=work/${b}_tgtodo.txt; : > $todo; for t in $(cat work/${b}_tg.txt); do [ -f work/g/${t}_D/$t.obj ] || echo $t >> $todo; done
bash tools/predef.sh $todo > work/defobj_$b.out 2>&1 &
BATCH=$b bash tools/runpair.sh $b $(cat batches/$b/rows.txt)
wait
bash tools/gates_bg.sh $b
echo done > work/run_$b.done
