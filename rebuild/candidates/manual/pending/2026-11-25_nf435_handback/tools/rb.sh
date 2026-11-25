#!/usr/bin/env bash
# rb.sh <addr...>: retail disassembly only (compact) + decl line
for a in "$@"; do D=$S/w22/agentNF431/dos/$a.txt; sed -n 1,2p $D | cut -c1-200; sed -n '/^-- retail/,/^-- FableWin/p' $D | grep -v "^-- " | awk '{$2=""; print}' | cut -c1-90; done
