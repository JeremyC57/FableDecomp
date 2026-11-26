#!/bin/bash
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP
cd $S/w22/agentNF461/q1link
export INCLUDE='Z:'$(echo $FABLE_VC71/include | sed 's#/#\\#g'); export LIB='Z:'$(echo $FABLE_VC71/lib | sed 's#/#\\#g')
CL="wine $FABLE_VC71/bin/cl.exe"; LK="wine $FABLE_VC71/bin/link.exe"; LB="wine $FABLE_VC71/bin/lib.exe"
for f in short full main; do $CL /nologo /c /O2 /Oy /W3 /Fo$f.obj $f.cpp; done
python3 ../tools/coffscan.py short.obj full.obj
$LB /nologo /out:short.lib short.obj; $LB /nologo /out:full.lib full.obj
lk() { n=$1; shift; echo "=== $n: link $*"; $LK /nologo /SUBSYSTEM:CONSOLE /MAP:$n.map /out:$n.exe "$@" 2>&1; grep -E 'Foo@@' $n.map | sed 's/  */ /g'; wine ./$n.exe; echo "exit=$?"; }
lk A_short_first main.obj short.obj full.obj
lk B_full_first main.obj full.obj short.obj
lk C_short_lib main.obj full.obj short.lib
lk D_full_lib main.obj short.obj full.lib
lk E_both_lib_shortfirst main.obj short.lib full.lib
lk F_both_lib_fullfirst main.obj full.lib short.lib
lk G_opt_noref_short_first /OPT:REF main.obj short.obj full.obj
