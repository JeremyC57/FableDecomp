#!/bin/bash
# ent.sh <tag> <python-replace-file>: make t8/v/<tag>.cpp from x2 with the replacement, compile (qc), print the entry roles
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent886
t=$1; base=${BASE:-x2}
python3 - "$A/t8/v/$base.cpp" "$A/t8/v/$t.cpp" "$2" <<'PY'
import sys
s = open(sys.argv[1]).read(); exec(open(sys.argv[3]).read())
open(sys.argv[2], 'w').write(s)
PY
timeout 900 bash $A/t8/qc.sh $A/t8/v/$t.cpp $t | tail -2
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad FABLE_OBJDUMP=objdump
echo "entry: $(timeout 120 python3 $A/t8/odis.py $A/t8/o/$t.obj 30 2>/dev/null | sed -n 6,13p | cut -c7- | tr '\n' ';')"
