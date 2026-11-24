#!/usr/bin/env bash
# lane365.sh <tag> <kind> <vardir> <addr...>: one background lane, rows in order, each via chain361.sh (one pass.py per row, own `timeout 900`).
# kind H -> H:<vardir>/<a>.cpp ; any other kind k -> k:<vardir>/<a>.cpp (object work/obj/<a>_<k>.obj)
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF365
t=$1; k=$2; d=$3; shift 3
for a in "$@"; do bash $A/tools/chain361.sh $t $a $k:$d/$a.cpp; done
echo "LANE_DONE $t $k $*" >> $A/work/lanes.log
