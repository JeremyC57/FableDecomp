#!/usr/bin/env python3
"""pcase.py <obj> <regex>: per-case (retail jump-table case) counts of instructions matching regex, retail v ours (via difflib
alignment of regs-masked keys to map our instructions to retail cases). Prints cases where counts differ."""
import sys, os, re, difflib, collections, bisect, struct
obj, rx = sys.argv[1], re.compile(sys.argv[2]); sys.argv = sys.argv[:2]
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
    return f'idx{starts[cs[i]]}@{cs[i]:#x}' if i >= 0 else 'entry'
sm = difflib.SequenceMatcher(None, [key(x, RET, 2) for x in R], [key(x, 0, 2) for x in O], autojunk=False)
omap = {}
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    for d in range(j2 - j1):
        omap[j1 + d] = R[min(i1 + (d if tag == 'equal' else 0), len(R) - 1)][0] + RET
rc = collections.Counter(case(x[0] + RET) for x in R if rx.search(f'{x[2]} {x[3]}'))
oc = collections.Counter(case(omap[j]) for j, x in enumerate(O) if rx.search(f'{x[2]} {x[3]}'))
for c in sorted(set(rc) | set(oc), key=lambda c: int(c.split('@')[1], 16) if '@' in c else 0):
    if rc[c] != oc[c]: print(f'{c:20s} retail {rc[c]:3d} ours {oc[c]:3d}')
print('total', sum(rc.values()), sum(oc.values()))
