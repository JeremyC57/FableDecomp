#!/bin/bash
# zsrun.sh <tag> <file>...: VC7.1 cl /Zs (zs.py) of each file in turn, each under its own timeout 900; log work/zs_<tag>.log (D1 precheck)
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
A=$S/w22/agentNF303; tag=$1; shift; cd $A/work
for f in "$@"; do
  r=$(timeout 900 python3 $A/tools/zs.py $f 2>&1 | head -c 600)
  echo "$(date +%T) $(echo "$r" | head -1 | cut -c1-40) $f" >> $A/work/zs_$tag.log
  echo "$r" | grep -q ZS_OK || echo "$r" | tail -3 >> $A/work/zs_$tag.log
done
echo ZS_DONE >> $A/work/zs_$tag.log
