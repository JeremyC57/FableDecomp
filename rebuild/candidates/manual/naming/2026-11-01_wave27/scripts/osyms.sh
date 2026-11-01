#!/bin/bash
# osyms.sh <ov> <grep-regex> <addr...>: compile each row's overlay source and list its defined (D) / undefined (U) symbols matching the regex
S=${S:?}; source $S/wine/env.sh >/dev/null 2>&1; W=$S/naming/w27; : "${W:?}"; ov=$1; re=$2; shift 2
for a in "$@"; do python3 $W/scripts/bld.py $ov $a $W/pw/b_$a >/dev/null || { echo "== $a nocompile"; continue; }
python3 -c "
import sys,re; sys.path.insert(0,'$W/scripts'); import coff; d,u=coff.syms('$W/pw/b_$a/a.obj'); print('== $a')
[print('  D',x) for x in sorted(d) if re.search(sys.argv[1],x)]; [print('  U',x) for x in sorted(u) if re.search(sys.argv[1],x)]" "$re"; done
