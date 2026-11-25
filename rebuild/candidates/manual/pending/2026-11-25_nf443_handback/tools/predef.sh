#!/usr/bin/env bash
# predef.sh <targets file>: clang precheck of each definer's HEAD source (D_<addr>), then VC7.1 objects of the passing ones (defobj.py)
W=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF443; ok=""
for a in $(cat $1); do s=$(python3 $W/tools/src.py $a | awk '{print $2}'); r=$(python3 $W/tools/nf.py pre D_$a $s nw | awk '{print $3}'); [ "$r" = PASS ] && ok="$ok $a" || echo "PREFAIL $a"; done
timeout 900 python3 $W/tools/defobj.py $ok
