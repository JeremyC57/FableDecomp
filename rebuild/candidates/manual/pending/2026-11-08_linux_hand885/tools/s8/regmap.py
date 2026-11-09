#!/usr/bin/env python3
"""regmap.py <m.obj> <m.asm> REG L1 L2: aligned instructions on source lines [L1, L2] where retail's operand is REG and ours's
aligned operand is something else (and vice versa with --ours): line, ours ins | retail ins."""
import sys
REGQ = sys.argv[3]; L1 = int(sys.argv[4]); L2 = int(sys.argv[5]); OURS = '--ours' in sys.argv
sys.argv = [sys.argv[0], sys.argv[1], sys.argv[2], '/dev/null']
src = open(__file__.replace('s8/tools/regmap.py', 'tools/pk/cdiff.py')).read()
src = src[:src.index('T = open(trace)')]
exec(src)
for k, ln in enumerate(lines):
    if not (L1 <= ln <= L2) or k not in pair: continue
    a, b = O[k], R[pair[k]]
    oa, ob = ops(a), ops(b)
    if len(oa) != len(ob): continue
    for x, y in zip(oa, ob):
        cx, cy = cls(x), cls(y)
        hit = (cx == REGQ and cy != REGQ) if OURS else (cy == REGQ and cx != REGQ)
        if hit:
            print(f'{ln:5d} {a.mnemonic} {a.op_str:36s} | {b.address:08x} {b.mnemonic} {b.op_str}'); break
