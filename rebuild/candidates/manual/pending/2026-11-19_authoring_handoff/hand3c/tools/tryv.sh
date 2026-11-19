#!/bin/bash
# tryv.sh <addr> <leaf> <src> <idea> : grade one variant, print grade + frame size + key slot lines
addr=$1; leaf=$2; src=$3; idea=$4
cd /home/user/FableDecomp; source $S/wine/env.sh >/dev/null 2>&1; export FABLE_HDR_OVERLAY=$W/hdr
timeout 500 python3 $W/tools/g.py $addr $src "$idea" >/dev/null 2>&1
tail -1 $W/ATTEMPTS.log
python3 $W/tools/dumpobj.py $addr "$leaf" > $W/tmp/m.txt 2>&1
