#!/bin/bash
# try.sh <overlay> <addr...>: quick recheck of a few rows from an overlay (tag try)
S=${S:?}; W=$S/naming/w30; : "${W:?}"; ov=$1; shift
printf '%s\n' "$@" > $W/runs/try.rows.txt
bash $W/scripts/recheck.sh try $ov $W/runs/try.rows.txt ${NP:-1} 2>&1 | tail -n 12
grep -E "^[0-9a-f]{8} " $W/runs/rk_try.log | grep -v WARN | cut -c1-200
