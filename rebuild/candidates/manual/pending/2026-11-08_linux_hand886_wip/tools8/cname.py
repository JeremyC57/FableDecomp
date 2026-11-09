#!/usr/bin/env python3
"""cname.py <hexaddr>...: the retail 0x0041E6D3 case label(s) whose switch target is the given case-start address (or the
nearest case start at or below it), from t8/cases.py's value -> target map."""
import sys, re
src = open('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent886/t8/cases.py').read()
src = src.split("src = open(sys.argv[1]).read()")[0]
args = sys.argv[1:]; sys.argv = ['x', 'x']; g = {}; exec(src, g)
H, E = g['handled'], g['enum']
starts = sorted(set(H.values()))
for a in args:
    a = int(a, 16); s = max([t for t in starts if t <= a] or [0])
    print(f'{a:#x}: case start {s:#x}', [E.get(v, v) for v, t in H.items() if t == s])
