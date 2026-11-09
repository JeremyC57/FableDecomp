#!/usr/bin/env python3
"""jm.py <obj>: every branch (jmp/jcc) whose target differs between retail and ours (targets compared through the
regs+frame-masked difflib alignment, ours mapped to retail VAs). Shows the retail branch, retail target, ours target (as
retail VA), with the jump-table case of each."""
import sys, os, re, difflib, bisect, struct
sys.argv = sys.argv[:2]
src = open(os.path.join(os.path.dirname(__file__), '..', 't7', 'm3.py')).read().split('fr = lambda')[0]
g = {'__name__': 'm3x'}; exec(compile(src, 'm3', 'exec'), g)
R, O, key, RET, retail = g['R'], g['O'], g['key'], g['RET'], g['retail']
sm = difflib.SequenceMatcher(None, [key(x, RET, 2) for x in R], [key(x, 0, 2) for x in O], autojunk=False)
o2r = {}; r2o = {}
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal':
        for d in range(i2 - i1): o2r[O[j1 + d][0]] = R[i1 + d][0] + RET; r2o[R[i1 + d][0] + RET] = O[j1 + d]
n = 0
for x in R:
    va = x[0] + RET
    if not (x[2].startswith('j') and re.match(r'^0x[0-9a-f]+$', x[3])): continue
    o = r2o.get(va)
    if o is None: continue
    rt = int(x[3], 16); ot = o2r.get(int(o[3], 16)) if re.match(r'^0x[0-9a-f]+$', o[3]) else None
    if rt != ot:
        n += 1; print(f'{va:#x} {x[2]:4s} retail -> {rt:#x}   ours -> {ot:#x}' if ot else f'{va:#x} {x[2]:4s} retail -> {rt:#x}   ours -> (unaligned +{int(o[3],16):#x})')
print(n, 'branches with a different target (aligned branches only)')
