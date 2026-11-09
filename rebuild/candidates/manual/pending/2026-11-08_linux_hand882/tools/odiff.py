#!/usr/bin/env python3
"""odiff.py <a.obj> <b.obj> [--mask none|frame|both] [-n N]: our-vs-our instruction diff of the row's function (frame / regs masked)."""
import sys, re, difflib
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
L = '?ConstructQuestListPC@CTCInventoryQuests@@QAEXPAVCComponent@NUISystem@@ABV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@PAVCList@3@W4QuestScreenType@@2@Z'
args = sys.argv[3:]
mode = args[args.index('--mask') + 1] if '--mask' in args else 'frame'
N = int(args[args.index('-n') + 1]) if '-n' in args else 8
md = Cs(CS_ARCH_X86, CS_MODE_32)
def key(i):
    o = i.op_str
    if re.match(r'^(j\w+|call)$', i.mnemonic) and re.match(r'^0x[0-9a-f]+$', o): o = 'T'
    if mode in ('frame', 'both'): o = re.sub(r'\[esp(?: [+-] (?:0x[0-9a-f]+|\d+))?\]', 'M', o)
    if mode == 'both': o = re.sub(r'\b(eax|ecx|edx|esi|edi|ebx|ebp)\b', 'R', o)
    return i.mnemonic + ' ' + o
def ins(o):
    b = V.obj_text(o, L)[0]; I = list(md.disasm(b, 0)); return [key(i) for i in I], I
A, IA = ins(sys.argv[1]); B, IB = ins(sys.argv[2])
sm = difflib.SequenceMatcher(None, A, B, autojunk=False); n = 0
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal': continue
    n += 1
    if n <= N:
        print(tag, 'a[%d:%d]@%x b[%d:%d]@%x' % (i1, i2, IA[i1].address if i1 < len(IA) else 0, j1, j2, IB[j1].address if j1 < len(IB) else 0))
        for k in range(i1, min(i2, i1 + 5)): print('   -', A[k])
        for k in range(j1, min(j2, j1 + 5)): print('   +', B[k])
print('differing opcodes', n, 'ins', len(A), len(B))
