#!/bin/bash
# hv2.sh <addr> <src> <tag> <basehdrdir> <hdrfile-rel> <pyfile>
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; D=$S/w22/chunk727
A=$1; SRC=$2; T=$3; BH=$4; HF=$5; PY=$6
rm -rf $D/work/hdr_$T; cp -r $BH $D/work/hdr_$T
python3 -c "
p='$D/work/hdr_$T/$HF'; s=open(p).read()
exec(open('$PY').read())
open(p,'w').write(s)"
L=$(sed -n 's,^// Leaf: ,,p' $SRC | head -1)
HDR=$D/work/hdr_$T bash $D/var.sh $A $SRC $T "$L" -
