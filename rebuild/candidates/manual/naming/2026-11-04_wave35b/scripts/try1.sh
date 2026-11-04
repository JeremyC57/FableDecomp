#!/bin/bash
# try1.sh <addr> <src> <leaf> [test]: grade one source read-only
SRC=$(readlink -f $2); TST=${4:+$(readlink -f $4)}
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
W=$S/wave35b; cd /home/user/FableDecomp
d=$W/try/$1; rm -rf $d; mkdir -p $d
printf '%s\t%s\t%s\t%s\t%s\n' $1 $SRC "$3" "x" "$TST" > $d/gen.tsv
python3 $W/scripts/mkpay.py $d $d/gen.tsv >/dev/null
LANDVERIFY_WORK=$d/work timeout 600 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $d/payload.json $d/oracle.tsv --recheck-landed 2>&1 | grep -v "^addr\|^$\|WINS"
python3 $W/scripts/objsyms.py $d/work $d/payload.json | cut -f2,3,4
