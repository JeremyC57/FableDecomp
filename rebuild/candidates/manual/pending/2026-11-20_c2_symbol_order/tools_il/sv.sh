#!/bin/bash
# sv.sh <name> <old> <new> [<old> <new>...] : variant of src/00b075d0.cpp (exact string replacements; "@def X" as old adds a #define X before the first include), then sym.sh + pp grade
name=$1; shift
python3 - "$W/src/00b075d0.cpp" "$W/probe/b075/s_$name.cpp" "$@" <<'PY'
import sys
s=open(sys.argv[1]).read(); a=sys.argv[3:]
for i in range(0,len(a),2):
    old,new=a[i],a[i+1]
    if old=='@def': s=s.replace('#include "fable_compiler.h"','#define '+new+'\n#include "fable_compiler.h"',1); continue
    if old not in s: print('MISSING:',old); sys.exit(1)
    s=s.replace(old,new,1)
open(sys.argv[2],'w').write(s)
PY
[ $? -ne 0 ] && exit 1
echo "== $name: $(python3 $W/tools/pp.py 00b075d0 $W/probe/b075/s_$name.cpp 2>&1 | tail -1)"
$W/tools/il/sym.sh $W/probe/b075/s_$name.cpp $name
