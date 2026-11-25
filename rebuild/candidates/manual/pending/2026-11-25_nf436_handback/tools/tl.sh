#!/usr/bin/env bash
# tl.sh <root> <row.cpp> <test.cpp> <tag> [row cl flags]: compile the row (/O2 /Oy /W3 + flags) and the test (/Od /W3), link, run under wine.
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; W=$S/w22/agentNF436
source $S/wine/env.sh >/dev/null; unset TMPDIR TMP; mkdir -p $W/work/cc; cd $W/work/cc
R=$1; r=$2; t=$3; g=$4; shift 4
python3 $W/tools/ccl.py $R $r r$g.obj "$@" | grep -E "error|rc [1-9]"
python3 - $R $t t$g.obj <<'PY' | grep -E "error|rc [1-9]"
import sys,os
os.environ["FABLE_ROOT"]=sys.argv[1]; sys.path.insert(0,sys.argv[1]+"/tools/decomp_pipeline"); import verify_and_land as V
r=V.cl(["/nologo","/c","/Od","/W3","/Fo"+sys.argv[3],sys.argv[2]],V.env()); print(r.stdout[-1500:],"rc",r.returncode)
PY
python3 $W/tools/ccl.py --link $R t$g.exe r$g.obj t$g.obj | grep -E "error|warn|rc [1-9]" | head -8
[ -f t$g.exe ] && wine t$g.exe
