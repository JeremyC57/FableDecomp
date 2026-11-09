#!/usr/bin/env python3
"""shape.py <m.obj> <m.asm> [--ctx]: code-shape hunks ours v retail with registers, frame offsets AND small constants
masked (push imm == push reg, cmp x,imm == cmp x,reg): what remains is not a register-allocation difference.
Prints each hunk with ours source lines."""
import sys, re, difflib
CTX = '--ctx' in sys.argv
sys.argv = [sys.argv[0], sys.argv[1], sys.argv[2], '/dev/null']
src = open(__file__.replace('s8/tools/shape.py', 'tools/pk/cdiff.py')).read()
exec(src[:src.index('sm = difflib')])
def k2(i):
    if JMP.match(i.mnemonic): return i.mnemonic + ' T'
    o = []
    for x in ops(i):
        if re.fullmatch(REG, x) or re.fullmatch(r'\[ebp(?: [+-] 0x[0-9a-f]+)?\]', x) or re.fullmatch(r'-?0x[0-9a-f]+|-?\d+', x): o.append('X')
        else: o.append(re.sub(REG, 'r', re.sub(r'0x[0-9a-f]+|(?<=\[)\d+(?=\])', 'k', x)))
    m = i.mnemonic
    if m in ('xor', 'or', 'mov') and len(o) == 2 and o == ['X', 'X']: m = 'MOVLIKE'
    return m + ' ' + ','.join(o)
KO = [k2(i) for i in O]; KR = [k2(i) for i in R]
sm2 = difflib.SequenceMatcher(None, KO, KR, autojunk=False)
n = 0
for tag, a1, a2, b1, b2 in sm2.get_opcodes():
    if tag == 'equal': continue
    n += 1
    ls = sorted(set(lines[a1:a2])) if a2 > a1 else [lines[a1] if a1 < len(lines) else -1]
    print('%s ours %d..%d (lines %s) retail %08x..%08x (%d v %d)' % (tag, a1, a2, ','.join(map(str, ls[:6])), R[b1].address if b1 < len(R) else 0, R[b2 - 1].address if b2 > b1 else 0, a2 - a1, b2 - b1))
    if CTX:
        for i in O[a1:a2][:6]: print('     O  %s %s' % (i.mnemonic, i.op_str))
        for i in R[b1:b2][:6]: print('     R  %s %s' % (i.mnemonic, i.op_str))
print('hunks', n)
