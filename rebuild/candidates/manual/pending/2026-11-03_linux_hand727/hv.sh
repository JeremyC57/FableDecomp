#!/bin/bash
# hv.sh <addr> <src> <tag> <hdrfile-rel> <pyexpr-file>: copy overlay, apply python edit (reads s, writes s) to one header, grade
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; D=$S/w22/chunk727
A=$1; SRC=$2; T=$3; HF=$4; PY=$5
rm -rf $D/work/hdr_$T; cp -r $D/hdr $D/work/hdr_$T
python3 -c "
import sys
p='$D/work/hdr_$T/$HF'; s=open(p).read()
exec(open('$PY').read())
open(p,'w').write(s)"
L=$(sed -n 's,^// Leaf: ,,p' $SRC | head -1)
HDR=$D/work/hdr_$T bash $D/var.sh $A $SRC $T "$L" -
