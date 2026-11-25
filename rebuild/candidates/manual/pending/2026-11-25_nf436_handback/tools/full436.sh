#!/usr/bin/env bash
# full436.sh <batch>: mkroots + runpair (vland H/N) + definer census + definer objects + gates436 + hdrcheck. Summary in work/full_<b>.txt
set -u
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; W=$S/w22/agentNF436; b=$1; cd $W
bash tools/mkroots.sh rs$b - batches/$b/hdr > /dev/null
bash tools/runpair432.sh $b rs$b > work/runpair_$b.out 2>&1
python3 tools/definers.py work/wd_${b}h,work/wd_${b}n batches/$b/rows.txt | tr ' ' '\n' > work/defs_$b.txt
TAG=def$b bash tools/hobj.sh $(cat work/defs_$b.txt) > work/mkdefs_$b.log 2>&1
bash tools/gates436.sh $b work/defs_$b.txt > work/full_$b.txt 2>&1
python3 tools/hdrcheck434.py $b | grep -vc SAME | sed 's/^/hdrcheck non-SAME: /' >> work/full_$b.txt
cat work/full_$b.txt
