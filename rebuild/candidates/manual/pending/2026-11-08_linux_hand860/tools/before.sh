#!/bin/bash
# before.sh <addr> <tag> <callee-substring> [n]: our disasm n lines before each call to the callee
A=$S/w22/agent860; n=${4:-8}
for off in $(python3 $A/bin/ocalls.py $1 $2 | grep -F "$3" | awk '{print $1}'); do
  a=$(printf "%x" $((0x$1 + 0x$off)))
  $A/bin/o.sh $1 $2 | grep -B$n "^0*$a " | awk '{$2="";print}'; echo --
done
