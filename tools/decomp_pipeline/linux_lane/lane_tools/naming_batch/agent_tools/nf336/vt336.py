#!/usr/bin/env python3
"""vt336.py <obj> <??_7 symbol> <retail vtable va> [retail slot count]: per slot, the emitted vftable's relocation symbol v the retail slot target address,
with the landed row Leaf at that address (catalog Source) and the reloc_bindings.tsv symbols bound there; the verdict per slot is
SAME when our symbol is the landed Leaf or a table symbol of that address, else CHECK. Also the slot counts. No bytes printed."""
import sys, struct, os, re
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF336'
sys.path.insert(0, A + '/tools'); import nf
obj, vsym, va = sys.argv[1], sys.argv[2], int(sys.argv[3], 16); NS = int(sys.argv[4]) if len(sys.argv) > 4 else None
secs, sl = nf.coff(obj)
sec = next(s for (i, n, v, s, t, c) in sl if n == vsym and s > 0)
s = secs[sec - 1]; n = len(s['data']) // 4; mine = {o // 4: sym for o, sym, t in s['rel']}
X = nf.exe() if hasattr(nf, 'exe') else None
d = open(os.environ['FABLE_EXE'], 'rb').read()
pe = struct.unpack_from('<I', d, 0x3C)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]; osz = struct.unpack_from('<H', d, pe + 20)[0]
base = struct.unpack_from('<I', d, pe + 52)[0]; so = pe + 24 + osz; SS = []
for i in range(ns):
    v_, vs, raw = struct.unpack_from('<I', d, so + i * 40 + 12)[0], struct.unpack_from('<I', d, so + i * 40 + 8)[0], struct.unpack_from('<I', d, so + i * 40 + 20)[0]
    SS.append((base + v_, vs, raw))
def rd(x):
    for b, l, r in SS:
        if b <= x < b + l: return struct.unpack_from('<I', d, r + x - b)[0]
tab = {}
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv', encoding='utf-8', errors='replace'):
    f = l.rstrip('\n').split('\t')
    if len(f) >= 2: tab.setdefault(f[1].lower(), set()).add(f[0])
def leafat(a):
    p = nf.src_of(a)
    if not p or not p.exists(): return None
    m = re.search(r'^// Leaf:\s*(\S+)', p.read_text(encoding='utf-8', errors='replace'), re.M)
    return m.group(1) if m else '(no Leaf)'
rt = 0
while True:
    t = rd(va + 4 * rt)
    if t is None or not (0x401000 <= t < 0x1200000): break
    rt += 1
if NS: rt = NS
print('##### %s %s v retail %08X: slots %d v %d' % (obj.split('/')[-1], vsym, va, n, rt))
for k in range(max(n, rt)):
    t = rd(va + 4 * k) if k < rt else None; a = '%08x' % t if t else '-'
    lf = leafat(a) if t else None; ts = tab.get(a, set())
    m = mine.get(k, '-')
    ok = t and (m == lf or m in ts)
    print('slot %d %s retail %s leaf=%s table=%s' % (k, 'SAME ' if ok else 'CHECK', a.upper(), lf, ','.join(sorted(ts)) or '-'))
    print('       ours %s' % m)
