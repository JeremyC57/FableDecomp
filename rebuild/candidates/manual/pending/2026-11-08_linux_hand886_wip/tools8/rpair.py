#!/usr/bin/env python3
"""rpair.py <obj>: in blocks equal under regs-masked keys, count (retail reg, ours reg) operand pairs that differ, per case."""
import sys, os, re, difflib, collections
sys.argv = sys.argv[:2]
src = open(os.path.join(os.path.dirname(__file__), '..', 't7', 'm3.py')).read().split('fr = lambda')[0]
g = {'__name__': 'm3x'}; exec(compile(src, 'm3', 'exec'), g)
R, O, key, RET = g['R'], g['O'], g['key'], g['RET']
RG = r'\b(eax|ecx|edx|esi|edi|ebx|al|cl|dl|bl)\b'
sm = difflib.SequenceMatcher(None, [key(x, RET, 1) for x in R], [key(x, 0, 1) for x in O], autojunk=False)
pairs = collections.Counter(); eq = 0; tot = 0
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag != 'equal': continue
    for d in range(i2 - i1):
        a = re.findall(RG, R[i1+d][3]); b = re.findall(RG, O[j1+d][3])
        for x, y in zip(a, b):
            tot += 1
            if x != y: pairs[(x, y)] += 1
            else: eq += 1
print(f'register operands in regs-equal blocks: {tot}, same {eq}, differ {tot-eq}')
for (x, y), n in pairs.most_common(30): print(f'  retail {x} / ours {y}: {n}')
