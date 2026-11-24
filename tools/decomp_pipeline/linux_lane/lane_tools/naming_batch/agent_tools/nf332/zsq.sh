#!/bin/bash
# zsq.sh <list> <donefile>: sequential D1 cl /Zs prechecks; each list line "tag file src|test [flags...]"; each on its own `timeout 900` line
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP   # NF332
A=$S/w22/agentNF332
while read -r tag f kind rest; do
  [ -z "$tag" ] && continue
  timeout 900 python3 $A/tools/nf.py zs $tag $f $kind $rest > /dev/null 2>&1 || echo "$(date +%T) ZS_FAIL timeout-or-error $tag $kind $f" >> $A/work/pre_zs.log
done < $1
echo done > $2
