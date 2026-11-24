#!/bin/bash
# zs_tests.sh <files tsv> <addr list> <donefile>: D1 VC7.1 cl /Zs of the listed rows' landed tests (nf.py zs, one at a time)
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
A=$S/w22/agentNF333
for a in $(cat $2); do t=$(grep "^$a" $1 | cut -f4); timeout 300 python3 $A/tools/nf.py zs T_$a "$t" test > /dev/null; done
echo done > $3
