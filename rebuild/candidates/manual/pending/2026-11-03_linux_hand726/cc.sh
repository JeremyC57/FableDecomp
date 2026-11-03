#!/bin/bash
# cc.sh <src.cpp> [sym-substring]: compile with lane flags (+overlay), disassemble the function containing sym
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S
C=$S/w22/chunk726; SRC=$1; SYM=$2
E=$(bash $C/pp.sh $SRC); if [ -n "$E" ]; then echo "PPFAIL $E"; exit 1; fi
w(){ echo "Z:$(echo $1 | sed 's,/,\\,g')"; }
export INCLUDE="$(w $FABLE_VC71/include);$(w $C/hdr);$(w /home/user/FableDecomp/rebuild/include)"
O=${SRC%.cpp}.obj
cd $(dirname $SRC)
timeout 100 wine $FABLE_VC71/bin/cl.exe /nologo /c /O2 /Oy /W3 "/Fo$(w $O)" "$(w $SRC)" > ${SRC%.cpp}.cl.txt 2>&1 || { grep -i error ${SRC%.cpp}.cl.txt | head; exit 1; }
if [ -n "$SYM" ]; then objdump -d -M intel --no-show-raw-insn $O | awk -v s="$SYM" '/^[0-9a-f]+ <.*>:$/{p=index($0,s)>0} p'; fi
