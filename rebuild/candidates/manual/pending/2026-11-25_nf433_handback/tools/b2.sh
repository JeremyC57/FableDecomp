#!/usr/bin/env bash
# b2.sh <addr...>: brief.sh without the HEAD row
for a in "$@"; do bash $S/w22/agentNF433/tools/brief.sh $a | sed '/^-- head/,$d'; done
