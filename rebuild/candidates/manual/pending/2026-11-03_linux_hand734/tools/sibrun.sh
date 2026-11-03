#!/bin/bash
# sibrun.sh: vland --recheck-landed every sibling batch on the full mirror, one at a time, 120 s each
export S=${S}; source $S/wine/env.sh >/dev/null 2>&1; export S
D=$S/w22/chunk734; cd /home/user/FableDecomp
for j in $D/sib/b*.json; do
  b=$(basename $j .json)
  LANDVERIFY_WORK=$D/lv/sib_$b FABLE_HDR_OVERLAY=$D/mirror timeout 120 python3 tools/decomp_pipeline/linux_lane/lane_tools/vland.py $j $D/sib/oracle.tsv --recheck-landed > $D/sib/out_$b.txt 2>&1
  echo "$b rc=$?"
done
echo ALLDONE
