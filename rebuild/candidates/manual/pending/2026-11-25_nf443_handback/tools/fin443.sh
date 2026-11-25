#!/usr/bin/env bash
# fin443.sh <batch>: name_gate, finalize, split HOLD rows out (release their locks), re-finalize, tablefix (compiled census), payfilter,
# xrcheck, screen, reserved_check, apply_table --dry (TABLE_CHANGES + CROSSBATCH)
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; W=$S/w22/agentNF443; b=$1; cd $W
source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP; export PYTHONDONTWRITEBYTECODE=1
timeout 1500 python3 tools/namegate438.py $b
python3 tools/finalize438.py $b $b $b
if grep -q -P "\tHOLD\t" batches/$b/RESULTS.tsv; then
  python3 tools/split438.py $b
  for a in $(cat batches/${b}_hold/rows.txt); do echo released > $S/w22/nflocks/$a/owner; done
  python3 tools/finalize438.py $b $b $b
fi
python3 tools/tablefix443.py $b
python3 tools/payfilter438.py $b
python3 tools/xrcheck438.py $b | tail -1; python3 tools/xr2_438.py $b | tail -1
python3 tools/screen438.py $b | tail -3
cut -f4 batches/$b/renames.tsv | sort -u > work/${b}_tg.txt
(cd /home/user/FableDecomp && RESERVED_REFS="" python3 tools/decomp_pipeline/linux_lane/lane_tools/coordinator/reserved_check.py $W/batches/$b/rows $W/work/${b}_tg.txt | head -12)
cat batches/$b/TABLE_CHANGES.tsv > work/${b}_tc_all.tsv; tail -n +2 batches/$b/CROSSBATCH.tsv >> work/${b}_tc_all.tsv
(cd /home/user/FableDecomp && python3 tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/apply_table.py $W/work/${b}_tc_all.tsv --dry)
cut -f2 batches/$b/RESULTS.tsv | sort | uniq -c
