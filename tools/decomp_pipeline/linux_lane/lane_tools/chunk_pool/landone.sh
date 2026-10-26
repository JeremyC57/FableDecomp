#!/bin/bash
# landone.sh <chunk> <nextchunk>: land a finished chunk, stage its parks, cut the next chunk + brief
export S=${S:?set S to your scratchpad}; source $S/wine/env.sh; cd /home/user/FableDecomp
n=$1; out=$(bash tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/land5.sh $n 2>&1 | grep -v ALLDONE); echo "$out"
if echo "$out" | grep -q "ASM in $n, skipped"; then echo "STOP: chunk $n not landed (purity grep); nothing staged, next chunk not cut"; exit 1; fi
grep -E "UNCHECKED|JUMPTABLE|IDENTITY" $S/lv$n.land.log | head -3
bash tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/stage_pending.sh agent$n 2026-10-24_linux_hand$n | tail -1
P=rebuild/candidates/manual/pending/2026-10-24_linux_hand$n; rm -rf $P/payload.json $P/oracle.tsv $P/src $P/test
cp $S/w22/agent$n/NOTES.md rebuild/candidates/manual/batches/2026-10-24_linux_hand$n/README.md
if [ -n "$2" ] && [ -e $S/w22/chunk$2.tsv ]; then echo "chunk $2 already cut: not recutting"; elif [ -n "$2" ]; then POOL=small python3 tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/mkchunk.py $2 1 241 1024 >/dev/null && timeout 900 python3 tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/mkprompt25.py $2 >/dev/null; echo "cut $2: $(tail -n +2 $S/w22/chunk$2.tsv | awk -F'\t' '{s+=$4} END{print NR" rows, mean "int(s/NR)" B"}')"; fi
echo "catalog $(awk 'NR>1' rebuild/oracles/auto-re-candidates.tsv | wc -l)"
