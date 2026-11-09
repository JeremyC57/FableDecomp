#!/usr/bin/env python3
"""lpair.py <m.obj> <m.asm> <line> [line2]: ours instructions of source lines [line..line2] (from the /FAs listing) next to
the retail instructions difflib pairs them with (key: registers and [ebp+x] masked)."""
import sys, re
sys.argv = [sys.argv[0], sys.argv[1], sys.argv[2], '/dev/null'] + sys.argv[3:]
L1 = int(sys.argv[4]); L2 = int(sys.argv[5]) if len(sys.argv) > 5 else L1
src = open(__file__.replace('lpair.py', 'cdiff.py')).read()
src = src[:src.index('T = open(trace)')]
exec(src)
lastr = None
for k, ln in enumerate(lines):
    if L1 <= ln <= L2:
        r = R[pair[k]] if k in pair else None
        print(f'{ln:5d} {k:6d} {O[k].mnemonic} {O[k].op_str:40s} | ' + (f'{r.address:08x} {r.mnemonic} {r.op_str}' if r else '--'))
