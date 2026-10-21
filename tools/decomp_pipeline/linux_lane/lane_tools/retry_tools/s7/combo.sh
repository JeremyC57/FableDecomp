#!/bin/bash
# combo.sh <addr>: chain every same-TU body generator that applies (def operator= bodies, growth, out-of-line push_back,
# near-callee stand-ins) into var/<addr>_s7cb.cpp and probe-grade it
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; cd /home/user/FableDecomp
W=$S/w22/retry10; a=$1; cur=$W/src/$a.cpp; steps=""
for g in "defreal.py $a s7c1" "grow2.py $a copy s7c2" "pb2.py $a s7c3" "lbreal2.py $a s7c5" "tmplbody.py $a s7c6" "nearbody.py $a s7c4"; do
  set -- $g; suf=${@: -1}
  r=$(SRCF=$cur python3 $W/s7/$g 2>&1 | tail -1)
  case "$r" in var/*) cur=$W/var/${a}_$suf.cpp; steps="$steps ${1%.py}";; esac
done
n=$(echo $steps | wc -w); [ "$n" -lt 2 ] && { echo -e "$a\tsingle\t$steps"; exit 0; }
cp $cur $W/var/${a}_s7cb.cpp
v=$(python3 $W/var/pv.py $a $W/var/${a}_s7cb.cpp 2>&1 | tail -1 | cut -c1-60)
echo -e "$a\t$v\t$steps"
