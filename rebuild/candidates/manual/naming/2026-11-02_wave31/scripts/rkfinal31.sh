#!/bin/bash
# rkfinal31.sh: the wave 31 final recheck in two parts (1596 rows > ~900): family rows + every landed includer of the three edited headers
S=${S:?}; W=$S/naming/w31; : "${W:?}"; cd $W || exit 1
bash scripts/recheck.sh finala ov_final $W/runs/final_p1.rows.txt 3 > logs/recheck_finala_driver.log 2>&1
bash scripts/recheck.sh finalb ov_final $W/runs/final_p2.rows.txt 3 > logs/recheck_finalb_driver.log 2>&1
cat runs/rk_finala.log runs/rk_finalb.log > runs/rk_final.log; cat runs/rk_finala.pairs.jsonl runs/rk_finalb.pairs.jsonl > runs/rk_final.pairs.jsonl
echo done
