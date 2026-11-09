#!/bin/bash
# srch7.sh <base.cpp> <tagprefix> <lever> [lever...]: one lever per variant (gen7.py reference bindings; a lever may be a
# comma list), serial plain compiles, score7 fitness -> s7/SEARCH.tsv (tag, lever, bytes, first-reg index, fx).
source /tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent885/tools/pk/env.sh
base=$(readlink -f $1); pre=$2; shift 2
for lev in "$@"; do
  tag=${pre}_$(echo $lev | tr ',' '+' | cut -c1-40)
  f=$A/s7/v/$tag.cpp
  python3 $A/tools/pk/gen7.py $base $f $lev > /dev/null
  (cd /home/user/FableDecomp && timeout 600 python3 $A/tools/mc.py $f $A/s7/qcw/$tag > $A/s7/qcw_$tag.log 2>&1) || { echo -e "$tag\t$lev\tFAIL" >> $A/s7/SEARCH.tsv; rm -rf $A/s7/qcw/$tag; continue; }
  sc=$(cd $A && python3 tools/pk/score7.py $A/s7/qcw/$tag/m.obj)
  echo -e "$(date +%H:%M)\t$tag\t$lev\t$sc" >> $A/s7/SEARCH.tsv
  rm -rf $A/s7/qcw/$tag $A/s7/qcw_$tag.log
done
echo search-done >> $A/s7/SEARCH.tsv
