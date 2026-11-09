#!/bin/bash
# fit.sh <src.cpp> <tag>: one compile through the c2 frame-packer probe (/FAs too) -> s6/fit/<tag>.{json,asm,obj};
# prints and appends to s6/LEADERBOARD.tsv: bytes, m3 exact/regs/frame, first differing instruction, frame items at retail offset
source /tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent885/tools/pk/env.sh
src=$(readlink -f $1); tag=$2; D=$A/s6/fit; mkdir -p $D
cd /home/user/FableDecomp
OBJ_OUT=$D/$tag.obj timeout 1500 python3 $A/tools/pk/pk.py $src $D/$tag.json > $D/$tag.log 2>&1 || { echo "$tag FAIL $(tail -3 $D/$tag.log)"; exit 1; }
m=$(python3 $A/tools/pk/m3x.py $D/$tag.obj)
f=$(python3 $A/tools/pk/first.py $D/$tag.obj)
p=$(python3 $A/tools/pk/pkmap.py $D/$tag.json $D/$tag.asm $D/$tag.obj --out $D/$tag.items.tsv | head -1 | sed 's/.*same offset //')
line="$tag	$(echo $m | cut -d' ' -f1)	$(echo $m | sed 's/.*exact /exact /')	frameitems $p	first $(echo $f | cut -c1-90)"
echo "$line"; echo "$(date +%H:%M) $line" >> $A/s6/LEADERBOARD.tsv
rm -f $D/$tag.asm.gz; gzip -f $D/$tag.asm
