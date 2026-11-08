#!/usr/bin/env python3
"""odiff.py <a.obj> <b.obj> [leaf]: instruction diff between two of OUR objects (frame offsets kept), with case labels from t2/order.txt via retail alignment omitted."""
import sys, os, re, difflib
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
leaf = sys.argv[3] if len(sys.argv) > 3 else '?Action@CManager@NUISystem@@'
md = Cs(CS_ARCH_X86, CS_MODE_32)
def L(p):
    b, sec, sym = V.obj_text(p, leaf)
    out = []
    for i in md.disasm(b, 0):
        op = i.op_str
        if re.match(r'^(j\w+|call)$', i.mnemonic) and re.match(r'^0x[0-9a-f]+$', op): op = 'T'
        out.append((i.address, f'{i.mnemonic} {op}'))
        if i.mnemonic == 'ret' and i.op_str == '4': break
    return out
A, B = L(sys.argv[1]), L(sys.argv[2])
sm = difflib.SequenceMatcher(None, [x[1] for x in A], [x[1] for x in B], autojunk=False)
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal': continue
    print(f'@@ {tag} a[{i1}:{i2}] +0x{A[i1][0] if i1 < len(A) else 0:x}  b[{j1}:{j2}] +0x{B[j1][0] if j1 < len(B) else 0:x}')
    for k in range(i1, min(i2, i1+8)): print('  -', A[k][1])
    for k in range(j1, min(j2, j1+8)): print('  +', B[k][1])
