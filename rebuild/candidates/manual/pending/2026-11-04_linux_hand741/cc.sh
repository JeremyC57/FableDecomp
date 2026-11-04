#!/bin/bash
# cc.sh <src> [flags]: compile with cl on the full mirror, objdump disasm to <src>.dis
export S=${S}; source $S/wine/env.sh >/dev/null 2>&1; export S
D=$S/w22/chunk741; MIR=${MIR:-$D/mirror}
SRC=$1; shift
w(){ echo "Z:$(echo $1 | sed 's,/,\\,g')"; }
M=$(python3 $S/w22/agent724b/incres.py $SRC $MIR $S/wine/vc71rtm/include | grep MISSING)
if [ -n "$M" ]; then echo "$M"; exit 1; fi
export INCLUDE="$(w $FABLE_VC71/include);$(w $MIR)"
O=${SRC%.cpp}.obj
timeout 110 wine $FABLE_VC71/bin/cl.exe /nologo /c /O2 /Oy /W3 "$@" /Fo"$(w $O)" "$(w $SRC)" > ${SRC%.cpp}.cl.txt 2>&1; echo rc=$?
grep -E "error|warning C4[0-9]*" ${SRC%.cpp}.cl.txt | head -20
objdump -dr -M intel --no-show-raw-insn $O > ${SRC%.cpp}.dis 2>/dev/null
