#!/bin/bash
# srcof.sh <addr> [test]: repo source (or TestSource) path of a landed row, from build_candidates.ps1
# NF315 fix: read only the row's own [pscustomobject] block (grep -A14 could return the NEXT entry's TestSource).
R=/home/user/FableDecomp; k=Source; d=src/compiled; [ "$2" = test ] && { k=TestSource; d=tests; }
p=$(awk -v a="Address = '$1'" 'index($0,a){f=1} f&&/^ *}/{exit} f' $R/rebuild/build_candidates.ps1 | grep -m1 -oP "\b$k = '\K[^']+"); [ -n "$p" ] && echo $R/rebuild/$d/$p
