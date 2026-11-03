#!/bin/bash
# cc.sh <src> [flags]: compile with cl (mirror include), objdump disasm to <src>.dis
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
D=$S/w22/chunk733
SRC=$1; shift
w(){ echo "Z:$(echo $1 | sed 's,/,\\,g')"; }
export INCLUDE="$(w $FABLE_VC71/include);$(w $D/mirror)"
O=${SRC%.cpp}.obj
timeout 120 wine $FABLE_VC71/bin/cl.exe /nologo /c /O2 /Oy /W3 "$@" /Fo"$(w $O)" "$(w $SRC)" > ${SRC%.cpp}.cl.txt 2>&1; echo rc=$?
objdump -d -M intel --no-show-raw-insn $O > ${SRC%.cpp}.dis 2>/dev/null
