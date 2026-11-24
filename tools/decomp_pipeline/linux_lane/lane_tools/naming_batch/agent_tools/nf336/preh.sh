#!/bin/bash
# preh.sh <files.tsv (addr src test)> <done>: clang precheck of every HEAD source (label H_<addr>) -> work/pre.log
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
A=$S/w22/agentNF336
while IFS=$'\t' read -r a s t rest; do
  timeout 120 python3 $A/tools/nf.py pre H_$a /home/user/FableDecomp/rebuild/src/compiled/$s > /dev/null
done < $1
echo done > $2
