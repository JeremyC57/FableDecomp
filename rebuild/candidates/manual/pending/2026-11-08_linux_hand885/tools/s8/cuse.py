#!/usr/bin/env python3
"""cuse.py <m.obj> <m.asm> [value]: every aligned instruction where ours and retail differ in how a small constant is
supplied (immediate v register) -- ours line, ours insn | retail insn; plus a per-line summary."""
import sys, re
VAL = sys.argv[3] if len(sys.argv) > 3 else None
sys.argv = [sys.argv[0], sys.argv[1], sys.argv[2], '/dev/null']
src = open(__file__.replace('s8/tools/cuse.py', 'tools/pk/cdiff.py')).read()
exec(src[:src.index('T = open(trace)')])
IMM = {'0xffffffff': '-1', '-1': '-1', '0': '0', '1': '1', '0xff': '-1b'}
REGS = {'esi', 'edi', 'ebx', 'si', 'di', 'bx', 'bl'}
for k, i in enumerate(O):
    if k not in pair: continue
    r = R[pair[k]]
    if i.mnemonic in ('call',) or i.mnemonic.startswith('j'): continue
    oo, rr = ops(i), ops(r)
    if len(oo) != len(rr): continue
    for p, (a, b) in enumerate(zip(oo, rr)):
        if (a in IMM and b in REGS) or (b in IMM and a in REGS):
            v = IMM.get(a) or IMM.get(b)
            if VAL and v != VAL: continue
            print('%5d %6d %-34s | %08x %s %s' % (lines[k], k, i.mnemonic + ' ' + i.op_str, r.address, r.mnemonic, r.op_str))
