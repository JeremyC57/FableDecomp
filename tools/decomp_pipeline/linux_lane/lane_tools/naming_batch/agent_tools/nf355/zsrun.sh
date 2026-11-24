#!/bin/bash
# zsrun.sh <tag> <file>...: VC7.1 cl /Zs (zs.py) of each file in turn, each under its own timeout 900; log work/zs_<tag>.log (D1 precheck)
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
A=$S/w22/agentNF355; tag=$1; shift; cd $A/work
for f in "$@"; do
  r=$(timeout 900 python3 -B $A/tools/zs.py $f 2>&1 | head -c 600)
  echo "$(date +%T) $(echo "$r" | head -1 | cut -c1-40) $f" >> $A/work/zs_$tag.log
  echo "$r" | grep -q ZS_OK || echo "$r" | tail -3 >> $A/work/zs_$tag.log
done
echo ZS_DONE >> $A/work/zs_$tag.log
