#!/bin/bash
# hh.sh <tag> [lo hi]: all difflib hunks of t8/o/<tag>.obj v retail (tools/cmp.py), minus the reloc-only artefacts
# (jump-table bases, absolute byte stores); optional retail VA window lo..hi (hex) keeps only hunks starting inside it.
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent886
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
cd /home/user/FableDecomp
L=$(grep -o '// Leaf: .*' $A/t8/v/x4.cpp | sed 's|// Leaf: ||')
python3 $A/tools/cmp.py $A/t8/o/$1.obj --leaf "$L" --hunks 400 --ctx 0 2>&1 | python3 -c '
import sys, re
lo = int(sys.argv[1], 16) if len(sys.argv) > 1 else 0; hi = int(sys.argv[2], 16) if len(sys.argv) > 2 else 1 << 40
out = []; keep = False; cur = []
for l in sys.stdin:
    if l.startswith("@@"):
        if keep and cur: out += cur
        m = re.search(r"\] 0x([0-9a-f]+) ", l); a = int(m.group(1), 16) if m else 0
        keep = lo <= a < hi and a < 0x426e0e; cur = [l]
    elif l.startswith("difflib"): print(l.rstrip())
    elif cur: cur.append(l)
if keep and cur: out += cur
txt = "".join(out)
txt = re.sub(r"@@[^\n]*\n(  [-+] [0-9a-f]+ (jmp dword ptr \[eax\*4[^\n]*|mov byte ptr \[0(x1[0-9a-f]+)?\], 0)\n)+(?=@@|$)", "", txt)
print(txt, end="")' ${2:-} ${3:-}
