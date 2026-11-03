#!/bin/bash
# rk.sh <outdir> <rowsfile> [nparts] : recheck landed rows (recheck.py payload, vland --recheck-landed), N parallel splits.
# Honours FABLE_HDR_OVERLAY from the environment.
S=${S:?}; source $S/wine/env.sh >/dev/null 2>&1; export S; cd /home/user/FableDecomp
F=${1:?}; R=${2:?}; N=${3:-4}; mkdir -p $F; rm -f $F/part_*
split -n l/$N -d $R $F/part_
for p in $F/part_0*; do b=$(basename $p); rm -rf $F/rk_$b; mkdir -p $F/rk_$b
  python3 tools/decomp_pipeline/linux_lane/lane_tools/recheck.py $F/rk_$b $(cat $p) > $F/rk_$b.mk.log
  ( LANDVERIFY_WORK=$F/rk_$b/work timeout 9000 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $F/rk_$b/payload.json $F/rk_$b/oracle.tsv --recheck-landed > $F/rk_$b.log 2>&1 ) &
done; wait; cat $F/rk_part_0*.log > $F/recheck.log
python3 $S/decisions/tools/grades.py $F
echo "done $(wc -l < $F/grades.tsv) graded"; awk -F'\t' '{print $2" "$3}' $F/grades.tsv | sort | uniq -c
