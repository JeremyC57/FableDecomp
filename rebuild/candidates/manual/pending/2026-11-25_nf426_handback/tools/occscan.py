#!/usr/bin/env python3
"""occscan.py: find every retail copy of the two family byte shapes and every reference to them -> evidence/occ.json
({addr: [shape, [[xref, kind], ...]]}). The shapes are read from the catalog oracle (auto-re-candidates.tsv) at two family rows
(T = 0x004044D0, 27 B, /O2; S = 0x00788B55, 24 B, size TU), never embedded. References: every E8/E9 rel32 in .text; absolute
dword hits over all sections are printed separately (evidence/absrefs.txt; all 9 found are inside call displacements)."""
import sys, os, re, json, struct, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); from pe import PE; import nf
cat = nf.catalog(); pats = {'T': bytes.fromhex(cat['004044d0']['bytes']), 'S': bytes.fromhex(cat['00788b55']['bytes'])}
p = PE(); va, t = p.text(); occ = {}
for k, pt in pats.items():
    for m in re.finditer(re.escape(pt), t): occ[va + m.start()] = k
calls = collections.defaultdict(list)
for i in range(len(t) - 5):
    if t[i] in (0xE8, 0xE9):
        tg = (va + i + 5 + int.from_bytes(t[i + 1:i + 5], 'little', signed=True)) & 0xffffffff
        if tg in occ: calls[tg].append(('%08x' % (va + i), 'call' if t[i] == 0xE8 else 'jmp'))
out = {'%08x' % a: [k, calls[a]] for a, k in occ.items()}
A = str(nf.A); json.dump(out, open(A + '/evidence/occ.json', 'w'), indent=0)
ab = open(A + '/evidence/absrefs.txt', 'w')
for nm, sva, vs, ro, rs in p.secs:
    d = p.d[ro:ro + rs]
    for i in range(len(d) - 3):
        v = struct.unpack_from('<I', d, i)[0]
        if v in occ: print(nm, "%08x" % (sva + i), "-> %08x" % v, file=ab)
ab.close()
print(len(occ), collections.Counter(occ.values()), 'with rel32 xrefs:', sum(1 for a in occ if calls[a]))
