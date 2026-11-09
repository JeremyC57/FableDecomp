#!/bin/bash
# runfx.sh tag..: qc.sh each s8/fx/<tag>.cpp serially (plain compile ~70 s), strins check, objects deleted after scoring.
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent885
cd $A; source tools/pk/env.sh
for t in "$@"; do
  r=$(timeout 700 bash tools/pk/qc.sh s8/fx/$t.cpp fx_$t 2>&1 | tail -1)
  s=$(python3 s8/tools/strins.py s7/qc_fx_$t.obj 2>/dev/null | head -1 | cut -c1-12)
  echo "$r	$s" | tee -a s8/fx/RESULTS.tsv
done
echo DONE
