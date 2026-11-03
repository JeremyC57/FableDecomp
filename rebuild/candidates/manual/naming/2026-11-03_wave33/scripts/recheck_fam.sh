#!/bin/bash
# recheck_fam.sh <famdir> <nparts>: split the OK rows, run vland --recheck-landed in parallel, concat logs
S=${S:?}; source $S/wine/env.sh >/dev/null 2>&1; cd /home/user/FableDecomp
F=$1; N=${2:-4}
grep -P "\tOK\t" $F/gen.tsv | cut -f1 > $F/ok.rows.txt
split -n l/$N -d $F/ok.rows.txt $F/part_
for p in $F/part_0*; do
  b=$(basename $p); mkdir -p $F/rk_$b
  python3 $S/wave33/scripts/mkpayload.py $F $F/rk_$b $(cat $p) >/dev/null
  ( LANDVERIFY_WORK=$F/rk_$b/work timeout 7000 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $F/rk_$b/payload.json $F/rk_$b/oracle.tsv --recheck-landed > $F/rk_$b.log 2>&1 ) &
done
wait
cat $F/rk_part_0*.log > $F/recheck.log
echo done
