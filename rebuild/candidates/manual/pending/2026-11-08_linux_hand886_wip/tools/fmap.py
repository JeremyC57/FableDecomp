#!/usr/bin/env python3
"""Frame-slot correspondence from the aligned (loose) instruction streams: frame.py
For every equal block of the register/frame-masked diff, pairs [ebp+X] displacements ours->retail and prints the votes,
sorted by our offset, so slot-order inversions (declaration order / size differences) show up."""
import sys, os, re, csv, difflib, collections, subprocess
csv.field_size_limit(10**8)
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = os.environ['S']; A = f'{S}/w22/agent886'
obj = sys.argv[1]
leaf = '?Action@CManager@NUISystem@@'
row = next(r for r in csv.DictReader(open(f'{S}/w22/chunk886.tsv', encoding='utf-8-sig'), delimiter='\t'))
retail = bytes.fromhex(row['bytes'])
built, sec, sym = V.obj_text(obj, leaf)
md = Cs(CS_ARCH_X86, CS_MODE_32)
R = [(i.mnemonic, i.op_str) for i in md.disasm(retail, 0x41E6D3)]
O = [(i.mnemonic, i.op_str) for i in md.disasm(built, 0)]
FR = re.compile(r'\[ebp(?: ([+-]) (0x[0-9a-f]+|\d+))?\]')
def offs(op):
    out = []
    for m in FR.finditer(op):
        if m.group(1) is None: out.append(0)
        else: out.append((-1 if m.group(1) == '-' else 1) * int(m.group(2), 16))
    return out
def key(mn, op):
    o = FR.sub('M', op)
    o = re.sub(r'0x[0-9a-f]{5,8}\b|\b0\b', 'K', o)
    o = re.sub(r'\b(eax|ecx|edx|esi|edi|ebx)\b', 'R', o); o = re.sub(r'\b(al|cl|dl|bl|ah|bh)\b', 'r', o)
    return mn + ' ' + o
sm = difflib.SequenceMatcher(None, [key(*x) for x in R], [key(*x) for x in O], autojunk=False)
votes = collections.defaultdict(collections.Counter)
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag != 'equal': continue
    for a, b in zip(range(i1, i2), range(j1, j2)):
        ro = offs(R[a][1]); oo = offs(O[b][1])
        if len(ro) == len(oo):
            for x, y in zip(oo, ro): votes[x][y] += 1
rows = []
for o in sorted(votes):
    (best, n), = votes[o].most_common(1)
    tot = sum(votes[o].values())
    rows.append((o, best, n, tot))
inv = collections.defaultdict(list)
for o, r, n, t in rows: inv[r].append(o)
for o, r, n, t in rows:
    flag = '' if n == t else f' (mixed {dict(votes[o].most_common(3))})'
    dup = ' DUP' if len(inv[r]) > 1 else ''
    print(f'ours {o:#8x} -> retail {r:#8x}  delta {r-o:+#x}  n={n}/{t}{flag}{dup}')
same = sum(n for o, r, n, t in rows if o == r); tot = sum(t for o, r, n, t in rows)
print(f'SUMMARY slots {len(rows)}: identical offset {sum(1 for o,r,n,t in rows if o==r)}; refs at identical offset {same}/{tot} = {same/max(tot,1):.1%}')
