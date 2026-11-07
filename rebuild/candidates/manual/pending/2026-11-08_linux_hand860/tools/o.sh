#!/bin/bash
# o.sh <addr> <tag> [from] [to] : our disasm, leaf from src/<addr>.cpp
A=$S/w22/agent860
L=$(grep -m1 '^// Leaf:' $A/src/$1.cpp | sed 's#^// Leaf: *##')
python3 $A/bin/odis.py $1 $2 "$L" $3 $4
