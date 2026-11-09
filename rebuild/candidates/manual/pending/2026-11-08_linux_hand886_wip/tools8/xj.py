#!/usr/bin/env python3
"""xj.py <obj>: cross-jumps = unconditional jmps whose target lies in ANOTHER case (retail jump-table case map), retail v ours
(ours mapped to retail VAs through the regs+frame-masked difflib alignment). Prints (src case -> dst case, dst insn)."""
import sys, os, re, difflib, bisect, struct, collections
sys.argv = sys.argv[:2]
src = open(os.path.join(os.path.dirname(__file__), '..', 't7', 'm3.py')).read().split('fr = lambda')[0]
g = {'__name__': 'm3x'}; exec(compile(src, 'm3', 'exec'), g)
R, O, key, RET, retail = g['R'], g['O'], g['key'], g['RET'], g['retail']
tab = 0x426E0E - RET; starts = {}; k = 0
while tab + 4 * k + 4 <= len(retail):
    t = struct.unpack_from('<I', retail, tab + 4 * k)[0]
    if not (RET <= t < RET + len(retail)): break
    starts.setdefault(t, k); k += 1
cs = sorted(starts)
def case(va):
    i = bisect.bisect_right(cs, va) - 1
    return f'{cs[i]:#x}' if i >= 0 else 'entry'
sm = difflib.SequenceMatcher(None, [key(x, RET, 2) for x in R], [key(x, 0, 2) for x in O], autojunk=False)
o2r = {}
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    for d in range(j2 - j1):
        o2r[O[j1 + d][0]] = R[min(i1 + (d if tag == 'equal' else 0), len(R) - 1)][0] + RET
END_R = 0x426dfc
def xjs(L, base, mapva):
    out = []
    offs = {x[0] for x in L}
    for x in L:
        if x[2] == 'jmp' and re.match(r'^0x[0-9a-f]+$', x[3]):
            t = int(x[3], 16) - (0 if base == RET else 0)
            tv = mapva(t); sv = mapva(x[0] if base == 0 else x[0])
            if tv is None or sv is None: continue
            if tv == END_R: continue
            if case(tv) != case(sv): out.append((case(sv), case(tv), tv))
    return out
r = xjs(R, RET, lambda a: a + RET if a < RET else a)
r = [(a, b, c) for a, b, c in xjs([(x[0] + RET, x[1], x[2], x[3]) for x in R], RET, lambda a: a)]
o = xjs(O, 0, lambda a: o2r.get(a))
rc, oc = collections.Counter(r), collections.Counter(o)
print('retail-only cross-jumps:'); [print('  ', a, '->', b, hex(c), n) for (a, b, c), n in sorted((rc - oc).items())]
print('ours-only cross-jumps:'); [print('  ', a, '->', b, hex(c), n) for (a, b, c), n in sorted((oc - rc).items())]
print('common', sum((rc & oc).values()))
if os.environ.get('XJALL'):
    print('ALL retail:'); [print('  ', a, '->', b, hex(c)) for (a, b, c) in sorted(r, key=lambda x: x[2])]
    print('ALL ours:'); [print('  ', a, '->', b, hex(c)) for (a, b, c) in sorted(o, key=lambda x: x[2])]
