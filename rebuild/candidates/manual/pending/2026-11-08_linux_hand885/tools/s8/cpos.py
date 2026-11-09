#!/usr/bin/env python3
"""cpos.py <m.obj> <m.asm>: where the callee-saved constant registers are materialised (or/xor/mov const) in ours and
retail, aligned (cdiff alignment): ours line, ours insn | retail insn."""
import sys, re
sys.argv = [sys.argv[0], sys.argv[1], sys.argv[2], '/dev/null']
src = open(__file__.replace('s8/tools/cpos.py', 'tools/pk/cdiff.py')).read()
exec(src[:src.index('T = open(trace)')])
inv = {v: k for k, v in pair.items()}
PAT = re.compile(r'^(or (esi|edi|ebx), 0xffffffff|xor (esi|edi|ebx), (esi|edi|ebx)|mov (esi|edi|ebx), (0x[0-9a-f]+|\d+))$')
for k, i in enumerate(O):
    s = i.mnemonic + ' ' + i.op_str
    if PAT.match(s):
        r = R[pair[k]] if k in pair else None
        print('O %5d %6d %-28s | %s' % (lines[k], k, s, ('%08x %s %s' % (r.address, r.mnemonic, r.op_str)) if r else '--'))
for j, r in enumerate(R):
    s = r.mnemonic + ' ' + r.op_str
    if PAT.match(s) and j not in inv:
        print('R  only %08x %s' % (r.address, s))
