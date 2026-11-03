#!/bin/bash
# tsweep.sh addr src sym T... : for each hidden threshold (diagnostic /d2inlT), the call-site offsets of symbol `sym` in the row function
I=${S}/pragma3
a=$1; f=$2; sym=$3; shift 3
for T in "$@"; do
  r=$($I/qq.sh $a $f --flags /d2inlT$T | head -1 | awk '{print $2}')
  n=$(objdump -d -r -M intel --no-show-raw-insn $I/work/$a/$(basename $f .cpp)/$a.obj | awk '/^[0-9a-f]+ <.*>:/{p=($0 !~ /<\$/) && (index($0,"'"$4"'")>0||1)} p' | grep -c "DISP32.*$sym")
  echo "T=$T $r n=$n"
done
