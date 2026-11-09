#!/usr/bin/env python3
"""cases.py: retail 0x0041E6D3 switch value -> target map (6 jump tables + the binary-search je's), compared with the `case`
labels of a source file (t8/cases.py <src.cpp>): prints values handled by retail but absent from the source and vice versa."""
import re, sys, csv, struct
csv.field_size_limit(10**8)
A = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent886'
row = next(r for r in csv.DictReader(open(A + '/../chunk886.tsv', encoding='utf-8-sig'), delimiter='\t'))
b = bytes.fromhex(row['bytes']); RET = 0x41E6D3; END = 0x426dfc
L = [l for l in open(A + '/dis_all.txt').read().split('\n') if re.match(r'^[0-9a-f]{8} ', l)]
ins = [(int(l[:8], 16), l[30:].strip()) for l in L]
vals = {}
# simulate the dispatch: track eax offset (sum of sub/dec/add) along the straight-line dispatch chains
for k, (a, t) in enumerate(ins):
    m = re.match(r'jmp dword ptr \[eax\*4 \+ (0x[0-9a-f]+)\]', t)
    if not m: continue
    tab = int(m.group(1), 16); base = 0; n = None
    for j in range(k - 1, max(k - 8, 0), -1):
        tt = ins[j][1]
        mm = re.match(r'cmp eax, (0x[0-9a-f]+|\d+)$', tt)
        if mm and n is None: n = int(mm.group(1), 0); continue
        mm = re.match(r'sub eax, (0x[0-9a-f]+|\d+)$', tt)
        if mm: base += int(mm.group(1), 0); continue
        mm = re.match(r'add eax, (-?0x[0-9a-f]+|-?\d+)$', tt)
        if mm: base -= int(mm.group(1), 0); continue
        if tt == 'dec eax': base += 1; continue
        if tt == 'inc eax': base -= 1; continue
        if not tt.startswith(('ja ', 'cmp')): break
    for i in range(n + 1):
        v = struct.unpack_from('<I', b, tab - RET + 4 * i)[0]
        vals[base + i] = v
# direct je targets after `cmp eax, X` (or cmp eax, reg with mov reg, X)
regs = {}
for k, (a, t) in enumerate(ins[:-1]):
    mm = re.match(r'mov (e[a-d]x), (0x[0-9a-f]+)$', t)
    if mm: regs[mm.group(1)] = int(mm.group(2), 16)
    mm = re.match(r'cmp eax, (0x[0-9a-f]+|\d+|e[a-d]x)$', t)
    if mm:
        x = mm.group(1); x = regs.get(x) if x.startswith('e') else int(x, 0)
        for j in (k + 1, k + 2):
            mj = re.match(r'je (0x[0-9a-f]+)$', ins[j][1])
            if mj and x is not None and a < 0x426dfc and x not in vals: vals[x] = int(mj.group(1), 16)
handled = {v: t for v, t in vals.items() if t != END}
enum = {}
for l in open('/home/user/FableDecomp/rebuild/include/engine/NUISystem_EActionType.h'):
    m = re.match(r'\s*(UI_ACTION\w*)\s*=\s*(\d+)', l)
    if m: enum.setdefault(int(m.group(2)), m.group(1))
src = open(sys.argv[1]).read()
ours = set(re.findall(r'case (UI_ACTION\w*):', src))
byname = {n: v for v, n in enum.items()}
print('retail handles', len(handled), 'values; source has', len(ours), 'case labels')
for v, t in sorted(handled.items()):
    n = enum.get(v, '?%d' % v)
    if n not in ours: print(f'  MISSING in source: {v} {n} -> {t:#x}')
for n in sorted(ours):
    v = byname.get(n)
    if v not in handled: print(f'  EXTRA in source: {v} {n}')
