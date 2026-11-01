#!/bin/bash
# ddiff.sh <ov> <addr> <len>: compile the overlay source of a row (row flags) and print its leaf function's disassembly next to retail
S=${S:?}; source $S/wine/env.sh; W=$S/naming/w17; ov=$1; a=$2; n=$3
cd /home/user/FableDecomp
src=$(grep -A4 "Address = '$a'" rebuild/build_candidates.ps1 | grep " Source" | sed "s/.*= '//;s/'//")
d=$W/pw/dd_$a; rm -rf $d; mkdir -p $d; cp $W/$ov/rebuild/src/compiled/$src $d/a.cpp
cd $d && python3 $W/pw/gw/cc.py a.cpp >/dev/null
objdump -d -M intel --no-show-raw-insn -r a.obj > a.dis
bash $W/scripts/dis.sh $a $n > r.dis
echo "== retail"; cat r.dis | cut -c1-90; echo "== built (all functions)"; grep -v "^$" a.dis | sed -n '5,400p' | cut -c1-110
