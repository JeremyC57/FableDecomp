#!/bin/bash
# zs_list.sh <tsv: tag file kind> <donefile>: D1 VC7.1 `cl /Zs` (nf.py zs) per line, each under timeout 300 -> work/pre_zs.log
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
A=$S/w22/agentNF339; cd $A/work
while read -r t f k; do timeout 300 python3 $A/tools/nf.py zs $t $f $k > /dev/null 2>&1; done < $1
echo done > $2
