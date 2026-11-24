#!/bin/bash
# srcof.sh <addr> [test]: repo source (or TestSource) path of a landed row, from build_candidates.ps1
R=/home/user/FableDecomp; k=Source; d=src/compiled; [ "$2" = test ] && { k=TestSource; d=tests; }
p=$(grep -A14 "Address = '$1'" $R/rebuild/build_candidates.ps1 | grep -m1 -oP "\b$k = '\K[^']+"); [ -n "$p" ] && echo $R/rebuild/$d/$p
