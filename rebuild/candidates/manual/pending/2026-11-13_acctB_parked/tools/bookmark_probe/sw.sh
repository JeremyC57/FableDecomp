#!/bin/bash
# sw.sh <addr> <leaf> <W> <src...>: compile each with q.py, print grade + bookmark block pattern diff v retail (probe)
export S=/opt/fable_lane; source $S/wine/env.sh >/dev/null 2>&1; export S; cd /home/user/FableDecomp
A=$1; L=$2; W=$3; shift 3
export FABLE_HDR_OVERLAY=${OV:-$W/hdr}
P=/opt/fable_lane/w22/bwf1/00b89c30/c2/pat.py
R=$(python3 $P $W/dis_all.txt)
for f in "$@"; do
  timeout 240 python3 $S/tools/q.py $A $f "$L" $FLAGS > $f.out 2>&1
  st=$(grep -m1 "^$A " $f.out | grep -o "RELOCATION_MATCH\|MATCH\|DIFFER([0-9v]*)\|SRC_FAIL" | head -1)
  O=$(python3 $P $f.out x 2>/dev/null)
  D=$(python3 -c "
import sys;r,o=sys.argv[1].split(),sys.argv[2].split()
d=[(i,b) for i,(a,b) in enumerate(zip(r,o)) if a!=b]
print(len(r),len(o),'ndiff',len(d)+abs(len(r)-len(o)),' '.join(f'{i}:{b}' for i,b in d))" "$R" "$O")
  echo "$(basename $f) $st $D"
  echo "$(date +%H:%M) $A probe $(basename $f) pattern -> $st $D" >> $W/ATTEMPTS.log
done
