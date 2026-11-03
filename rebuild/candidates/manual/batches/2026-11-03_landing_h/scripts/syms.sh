#!/bin/bash
# syms.sh <landed rel source> <pattern>: compile on the tree, print matching symbols (D=defined, U=undefined)
source $S/wine/env.sh >/dev/null 2>&1
SRC=/home/user/FableDecomp/rebuild/src/compiled/$1; O=$S/land0311h/obj/$(basename ${1%.cpp}).obj
w(){ echo "Z:$(echo $1 | sed 's,/,\\,g')"; }
export INCLUDE="$(w $FABLE_VC71/include);$(w /home/user/FableDecomp/rebuild/include)"
timeout 150 wine $FABLE_VC71/bin/cl.exe /nologo /c /O2 /Oy /W3 $3 /Fo"$(w $O)" "$(w $SRC)" > $O.cl.txt 2>&1 || { echo "CLFAIL"; grep error $O.cl.txt|head -3; }
objdump -t $O | grep -E "\(scl +2\)" | grep -E "$2" | sed -E 's/.*\(sec +([-0-9]+)\).*\(nx 0\) 0x[0-9a-f]+ (.*)/\1 \2/' | awk '{print ($1=="0"?"U ":"D ") $2}' | sort -u
