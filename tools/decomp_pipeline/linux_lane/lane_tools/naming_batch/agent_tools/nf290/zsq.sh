#!/bin/bash
# zsq.sh <list> <donefile>: sequential D1 cl /Zs prechecks; each list line "tag file src|test [flags...]"; each on its own `timeout 900` line
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
A=$S/w22/agentNF290
while read -r tag f kind rest; do
  [ -z "$tag" ] && continue
  timeout 900 python3 $A/tools/nf.py zs $tag $f $kind $rest > /dev/null 2>&1 || echo "$(date +%T) ZS_FAIL timeout-or-error $tag $kind $f" >> $A/work/pre_zs.log
done < $1
echo done > $2
