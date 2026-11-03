#!/bin/bash
# cc.sh <mirror> <src> <outobj>: compile with cl RTM on a full header mirror; print undefined externals
source $S/wine/env.sh >/dev/null 2>&1
MIR=$1; SRC=$2; O=$3
w(){ echo "Z:$(echo $1 | sed 's,/,\\,g')"; }
export INCLUDE="$(w $FABLE_VC71/include);$(w $MIR)"
timeout 150 wine $FABLE_VC71/bin/cl.exe /nologo /c /O2 /Oy /W3 /Fo"$(w $O)" "$(w $SRC)" > $O.cl.txt 2>&1; echo rc=$?
grep -E " error " $O.cl.txt | head -5
objdump -t $O | grep -E "UND" | awk '{print $NF}' | grep -E "TransformPoint|Interpolate@CRGBFloatColour|GetRGBColourClamped|TransformPosIfWorld" 
