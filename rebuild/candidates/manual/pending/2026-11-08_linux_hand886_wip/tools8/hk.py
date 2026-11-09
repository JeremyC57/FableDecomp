#!/usr/bin/env python3
"""hk.py <obj> [mode] [--show N]: difflib hunks of retail 0x0041E6D3 v ours under t7/m3.py keys (mode 0 exact, 1 regs masked,
2 regs+frame masked). mode 1 lists STRUCTURAL hunks; mode 0 restricted to blocks equal under mode 1 = REGISTER-only hunks.
Each hunk: retail VA range, size, the jump-table case it falls in (retail table 0x00426E0E)."""
import sys, os, re, difflib, bisect
sys.argv, args = sys.argv[:2], sys.argv[2:]
mode = int(args[0]) if args and args[0].isdigit() else 1
show = int(args[args.index('--show') + 1]) if '--show' in args else 0
import importlib.util
spec = importlib.util.spec_from_file_location('m3', os.path.join(os.path.dirname(__file__), '..', 't7', 'm3.py'))
src = open(spec.origin).read().split('fr = lambda')[0]
g = {'__name__': 'm3x'}; exec(compile(src, 'm3', 'exec'), g)
R, O, key, retail, RET = g['R'], g['O'], g['key'], g['retail'], g['RET']
# case starts from the retail jump table
import struct
tab = 0x426E0E - RET; starts = {}
k = 0
while tab + 4 * k + 4 <= len(retail):
    t = struct.unpack_from('<I', retail, tab + 4 * k)[0]
    if not (RET <= t < RET + len(retail)): break
    starts.setdefault(t, k); k += 1
cs = sorted(starts)
def case(va):
    i = bisect.bisect_right(cs, va) - 1
    return f'idx{starts[cs[i]]}@{cs[i]:#x}' if i >= 0 else 'entry'
def hunks(m):
    sm = difflib.SequenceMatcher(None, [key(x, RET, m) for x in R], [key(x, 0, m) for x in O], autojunk=False)
    return sm
if mode == 0:
    sm1 = hunks(1)
    out = []
    for tag, i1, i2, j1, j2 in sm1.get_opcodes():
        if tag != 'equal': continue
        for d in range(i2 - i1):
            a, b = R[i1 + d], O[j1 + d]
            if key(a, RET, 0) != key(b, 0, 0): out.append((i1 + d, j1 + d))
    # group consecutive
    grp = []
    for i, j in out:
        if grp and i == grp[-1][1] + 1 and j == grp[-1][3] + 1: grp[-1][1] = i; grp[-1][3] = j
        else: grp.append([i, i, j, j])
    print(f'register-only differing ins: {len(out)} in {len(grp)} runs')
    for n, (i1, i2, j1, j2) in enumerate(grp):
        va = R[i1][0] + RET
        print(f'{va:#x} n={i2-i1+1} {case(va)}  ' + ' ; '.join(f'{R[i][2]} {R[i][3]} / {O[j][3]}' for i, j in zip(range(i1, i2+1), range(j1, j2+1)))[:200])
else:
    sm = hunks(mode); tot = 0; n = 0
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal': continue
        n += 1; tot += max(i2 - i1, j2 - j1)
        va = R[i1][0] + RET if i1 < len(R) else 0
        print(f'{tag:7s} retail {va:#x} [{i2-i1}] ours +{O[j1][0] if j1 < len(O) else 0:#x} [{j2-j1}]  {case(va)}')
        if show:
            for x in R[i1:i1+show] if i2 > i1 else []: print(f'    - {x[0]+RET:#x} {x[2]} {x[3]}')
            for x in O[j1:j1+show] if j2 > j1 else []: print(f'    + {x[0]:#x} {x[2]} {x[3]}')
    print(f'{n} hunks, {tot} ins')
