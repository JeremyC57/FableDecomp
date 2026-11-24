#!/bin/bash
# preov.sh <overlay-dir> <src>...: clang -fsyntax-only precheck with an overlay include dir before rebuild/include (NF355); logs to work/pre.log
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; A=$S/w22/agentNF359; OV=$1; shift
for f in "$@"; do
  e=$(cd $A/work && timeout 120 clang++ -target i686-pc-windows-msvc -fms-extensions -fms-compatibility -fms-compatibility-version=13.10 -fsyntax-only -w ${NOWCHAR:+-Xclang -fno-wchar} -I$OV -I/home/user/FableDecomp/rebuild/include -isystem $S/wine/vc71/include "$f" 2>&1 | grep -E "error" )
  if [ -z "$e" ]; then r=PASS; else r=FAIL; fi
  echo "$(date +%H:%M:%S) preov $r ov=$OV $f" | tee -a $A/work/pre.log; [ -n "$e" ] && echo "$e" | head -${N:-6}
done; true
