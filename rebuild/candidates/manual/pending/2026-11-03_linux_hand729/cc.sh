#!/bin/bash
# cc.sh <src.cpp> [cl flags]: compile with overlay $C/hdr first; kill on fatal error (Wine cl spins after C1083/C1189)
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S
C=$S/w22/chunk729; SRC=$1; shift
w(){ echo "Z:$(echo $1 | sed 's,/,\\,g')"; }
export INCLUDE="$(w $FABLE_VC71/include);$(w ${OV:-$C/hdr});$(w /home/user/FableDecomp/rebuild/include)"
OUT=${SRC%.cpp}.out; OBJ=${SRC%.cpp}.obj; rm -f $OBJ
wine $FABLE_VC71/bin/cl.exe /nologo /c /O2 /Oy /W3 "$@" /Fo"$(w $OBJ)" "$(w $SRC)" > $OUT 2>&1 &
P=$!
for i in $(seq 1 200); do
  sleep 0.5
  if ! kill -0 $P 2>/dev/null; then break; fi
  if grep -q "fatal error" $OUT; then kill -9 $P; echo KILLED-FATAL; break; fi
done
kill -0 $P 2>/dev/null && { kill -9 $P; echo KILLED-TIMEOUT; }
grep -E "error|warning C4[0-9]*" $OUT | head -30; ls -la $OBJ 2>/dev/null | awk '{print "obj",$5}'
