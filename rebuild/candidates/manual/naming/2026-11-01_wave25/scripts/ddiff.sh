#!/bin/bash
# ddiff.sh <ov> <addr> <len> [leafgrep]: compile the overlay source of a row (row flags) and print the leaf's disassembly next to retail
S=${S:?}; source $S/wine/env.sh; W=$S/naming/w25; ov=$1; a=$2; n=$3
cd /home/user/FableDecomp
d=$W/pw/dd_$a
python3 $W/scripts/bld.py $ov $a $d || exit 1
objdump -d -M intel --no-show-raw-insn -r $d/a.obj > $d/a.dis
bash $W/scripts/dis.sh $a $n > $d/r.dis
echo "== retail"; cat $d/r.dis | cut -c1-90; echo "== built (all functions)"; grep -v "^$" $d/a.dis | sed -n '5,400p' | cut -c1-110
