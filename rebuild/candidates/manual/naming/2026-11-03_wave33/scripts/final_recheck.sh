#!/bin/bash
S=${S:?}; source $S/wine/env.sh >/dev/null 2>&1; cd /home/user/FableDecomp
F=$S/wave33/final; rm -rf $F; mkdir -p $F
cut -f2 $S/wave33/applied_all.tsv > $F/rows.txt
split -n l/4 -d $F/rows.txt $F/part_
for p in $F/part_0*; do b=$(basename $p); mkdir -p $F/rk_$b
  python3 tools/decomp_pipeline/linux_lane/lane_tools/recheck.py $F/rk_$b $(cat $p) > $F/rk_$b.mk.log
  ( LANDVERIFY_WORK=$F/rk_$b/work timeout 7000 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $F/rk_$b/payload.json $F/rk_$b/oracle.tsv --recheck-landed > $F/rk_$b.log 2>&1 ) &
done; wait; cat $F/rk_part_0*.log > $F/recheck.log; echo done
