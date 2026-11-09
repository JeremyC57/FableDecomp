#!/usr/bin/env python3
"""cmap.py <obj> <listing.asm> [retail VA ...]: map retail case addresses to OUR source line / `case` label through the /FAs
listing (instruction order) and the regs+frame-masked difflib alignment. Without VAs: print every retail jump-table case start."""
import sys, os, re, difflib, struct, bisect
obj, asm = sys.argv[1], sys.argv[2]; vas = [int(x, 16) for x in sys.argv[3:]]
sys.argv = sys.argv[:2]
src = open(os.path.join(os.path.dirname(__file__), '..', 't7', 'm3.py')).read().split('fr = lambda')[0]
g = {'__name__': 'm3x'}; exec(compile(src, 'm3', 'exec'), g)
R, O, key, RET, retail = g['R'], g['O'], g['key'], g['RET'], g['retail']
# listing: instruction lines with the latest source line / case label
ins = []; inside = False; line = None; case = None
for l in open(asm, encoding='latin1'):
    if re.match(r'^\?Action@CManager@NUISystem@@\S* PROC', l): inside = True; continue
    if inside and re.match(r'^\?Action@CManager@NUISystem@@\S* ENDP', l): break
    if not inside: continue
    m = re.match(r'^; (\d+)\s*:\s*(.*)$', l)
    if m:
        line = int(m.group(1)); t = m.group(2)
        mc = re.search(r'\bcase (\w+)\s*:', t) or re.search(r'\b(default)\s*:', t)
        if mc: case = (line, mc.group(1))
        continue
    m = re.match(r'^\t([a-z]+)\b', l)
    if m and m.group(1) not in ('npad', 'dd', 'db', 'align'):
        ins.append((m.group(1), line, case))
om = [x[2] for x in O]
sm = difflib.SequenceMatcher(None, [x[0] for x in ins], om, autojunk=False)
o2l = {}
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal':
        for d in range(i2 - i1): o2l[j1 + d] = ins[i1 + d]
sm2 = difflib.SequenceMatcher(None, [key(x, RET, 2) for x in R], [key(x, 0, 2) for x in O], autojunk=False)
r2o = {}
for tag, i1, i2, j1, j2 in sm2.get_opcodes():
    for d in range(i2 - i1): r2o[R[i1 + d][0] + RET] = j1 + min(d, max(j2 - j1 - 1, 0))
if not vas:
    tab = 0x426E0E - RET; k = 0; seen = set()
    while tab + 4 * k + 4 <= len(retail):
        t = struct.unpack_from('<I', retail, tab + 4 * k)[0]
        if not (RET <= t < RET + len(retail)): break
        seen.add(t); k += 1
    vas = sorted(seen)
for va in vas:
    j = r2o.get(va); x = o2l.get(j) if j is not None else None
    print(f'{va:#x} -> ours +{O[j][0]:#x} line {x[1]} case {x[2]}' if x else f'{va:#x} -> ?')
