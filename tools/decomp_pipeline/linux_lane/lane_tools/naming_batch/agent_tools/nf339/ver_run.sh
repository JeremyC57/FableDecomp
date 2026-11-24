#!/bin/bash
# ver_run.sh <tag>: vland HEAD then new at ONE work path ($D/wd), each moved aside to $D/<side>/wd (the ?A0x hash follows the path)
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
A=$S/w22/agentNF339; W=$A/work; D=$W/v_$1; cd $W
for side in head new; do
  [ -e $D/wd ] && { echo "stale $D/wd" > $D/err; exit 1; }
  PYTHONPATH=/home/user/FableDecomp/tools/decomp_pipeline:/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools LANDVERIFY_WORK=$D/wd timeout 900 python3 -u /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/vland.py $D/$side/payload.json $D/$side/oracle.tsv --recheck-landed > $D/$side/vland.log 2>&1
  mv $D/wd $D/$side/wd
done
echo done > $D/done
