#!/usr/bin/env bash
# run431.sh <tag> <rowsdir> <addr...>: HEAD payload (recheck.py), new + mutant payloads (mkpay.py), then vland h / n / m at ONE work path.
# Logs work/vland_<tag>{h,n,m}.log
source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP
W=$S/w22/agentNF431; tag=$1; rd=$2; shift 2
LT=/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools; CL=$LT/naming_batch/clean.py
cd /home/user/FableDecomp; mkdir -p $W/work/p_${tag}h
python3 $LT/recheck.py $W/work/p_${tag}h "$@" >/dev/null || exit 1
python3 $W/tools/mkpay.py $W $tag $rd "$@" || exit 1
cd $W/work/rsroot; export FABLE_HDR_OVERLAY=$W/hdr
for k in h n m; do
  python3 $CL $W $W/work/wd_$tag >/dev/null
  timeout 3600 env LANDVERIFY_WORK=$W/work/wd_$tag python3 -u $LT/vland.py $W/work/p_${tag}${k}/payload.json $W/work/p_${tag}${k}/oracle.tsv --recheck-landed > $W/work/vland_${tag}${k}.log 2>&1
  python3 $CL $W $W/work/wd_${tag}${k} >/dev/null; mv $W/work/wd_$tag $W/work/wd_${tag}${k}
done
echo RUN_DONE $tag
