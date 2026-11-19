#!/bin/bash
# tce.sh <addr>: show compile errors of the last mt.sh test build
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export FABLE_HDR_OVERLAY=$S/w24/hand4d/hdr; cd /home/user/FableDecomp
A=$1 timeout 300 python3 - <<'PY' 2>&1 | grep -i "error\|warning C4" | head -20
import sys,os
sys.path.insert(0,"tools/decomp_pipeline")
import verify_and_land as V
from pathlib import Path
a=os.environ["A"]; w=Path(os.environ["S"])/"w24/hand4d/tmp/mt"/a
r=V.cl(["/nologo","/c","/Od","/W3",f"/Fo{w}/x.obj",str(w/(a+".test.cpp"))],V.env())
print(r.stdout[-6000:])
PY
