#!/bin/bash
# fx.sh <src.cpp> [tag]: compile-only (/FAs) -> work/s7/fit/<tag>.obj/.asm, prints bytes, strict/loose/noframe, call count,
# frame size and the continue-layout markers. cwd = work/s7 (never the repo).
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
A=$S/w22/agent890; W=$A/work/s7; cd $W
src=$(readlink -f $1); tag=${2:-$(basename $src .cpp)}; D=$W/fit; mkdir -p $D/tmp_$tag
cp $src $D/tmp_$tag/$tag.cpp
FABLE_HDR_OVERLAY=$A/hdr timeout 300 python3 $A/work/fas.py $D/tmp_$tag/$tag.cpp $D/tmp_$tag > $D/$tag.log 2>&1 || { echo "$tag COMPILE_FAIL"; tail -5 $D/$tag.log; rm -rf $D/tmp_$tag; exit 1; }
mv $D/tmp_$tag/$tag.obj $D/$tag.obj; mv $D/tmp_$tag/$tag.asm $D/$tag.asm 2>/dev/null; rm -rf $D/tmp_$tag
l1=$(timeout 120 python3 $W/tools/dd7.py 0056ad40 $D/$tag.obj --ctx 0 | head -1 | sed 's/.*ours //')
nc=$(timeout 60 python3 $W/tools/ncalls.py $D/$tag.obj)
echo "$tag | $l1 | $nc"
echo "$(date +%H:%M:%S) $tag | $l1 | $nc" >> $W/LEADER.txt
gzip -f $D/$tag.asm 2>/dev/null
