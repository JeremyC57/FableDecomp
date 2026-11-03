#!/bin/bash
# clx.sh <file.cpp> [extra cl args]: compile one file with the lane cl (RTM 3077) against rebuild/include (+ FABLE_HDR_OVERLAY first); prints errors.
S=${S:?}; source $S/wine/env.sh >/dev/null 2>&1
f=$(readlink -f $1); shift; d=$(dirname $f); cd $d
INC="/IZ:/home/user/FableDecomp/rebuild/include"; [ -n "$FABLE_HDR_OVERLAY" ] && INC="/IZ:$(readlink -f $FABLE_HDR_OVERLAY) $INC"
export INCLUDE="Z:$FABLE_VC71/include" LIB="Z:$FABLE_VC71/lib"
PATH=$FABLE_VC71/bin:$PATH timeout 100 wine $FABLE_VC71/bin/cl.exe /nologo /c /O2 /Oy /W3 $INC "$@" "Z:$f" 2>&1 | grep -vE '^\s*$' | head -30
