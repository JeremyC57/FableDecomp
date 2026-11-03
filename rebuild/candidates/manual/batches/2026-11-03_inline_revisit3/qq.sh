#!/bin/bash
# qq.sh <addr> <src> [qq.py args]; full header mirror (tree d0f753026) + optional overlay hdr
export S=${S}
source $S/wine/env.sh >/dev/null 2>&1; export S
a1=$1; shift; f=$(readlink -f "$1"); shift; cd /home/user/FableDecomp
FABLE_HDR_OVERLAY=${MIR:-$S/pragma3/mirror} timeout 300 python3 $S/pragma3/qq.py $a1 "$f" "$@"
