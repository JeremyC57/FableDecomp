#!/usr/bin/env python3
"""rtti428.py: a retail-only source for each Reset row's T, independent of the table, the tracker and the PDB layouts. For every retail
call site of the row (evidence/xref.json), the window from the previous call of the same row (or the caller's start, max 0x300 bytes)
up to the call is scanned for (a) direct calls to a function the tracker names ??0<X>@@ (the new object's constructor) and (b)
`mov dword [reg], imm32` / `mov dword [reg+0], imm32` stores whose imm32 is an RTTI vftable (rebuild/oracles/data_vas.tsv `RTTI .?AV<X>@@`),
following each called ctor one level (its own vftable stores). RTTI_OK: every site names exactly the row's T (most-derived class X == T);
RTTI_DERIVED: X differs (a class derived from T, or another object); NONE: nothing readable. -> evidence/rtti.tsv"""
import sys, os, json, glob, re, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); from pe import PE; import nf
A = str(nf.A); x = json.load(open(A + '/evidence/xref.json')); p = PE(); cat = nf.catalog(); r = json.load(open(A + '/status/rec.json'))
vt = {}
for l in open('/home/user/FableDecomp/rebuild/oracles/data_vas.tsv'):
    f = l.rstrip('\n').split('\t')
    if len(f) >= 4 and f[2] == 'vftable' and f[3].startswith('RTTI .?AV'): vt[int(f[0], 16)] = f[3][9:]
def mang(ct):
    c = ct.replace('const ', ''); parts = c.split('::'); return '@'.join(reversed(parts)) + '@@'
def scan(lo, hi, depth=1):
    b = p.read(lo, hi - lo); found = []
    for m in re.finditer(rb'\xc7[\x00-\x07]', b):           # mov dword [reg], imm32
        i = m.start()
        if i + 6 <= len(b):
            v = int.from_bytes(b[i + 2:i + 6], 'little')
            if v in vt: found.append(vt[v])
    for i in range(len(b) - 4):
        if b[i] == 0xE8:
            t = (lo + i + 5 + int.from_bytes(b[i + 1:i + 5], 'little', signed=True)) & 0xffffffff
            e = r.get('%08x' % t, {}); n = e.get('pdb_spelling') or (e.get('names') or [''])[0]
            if n.startswith('??0'): found.append('ctor:' + n[3:].split('@@QAE')[0] + '@@')
            if depth and '%08x' % t in cat and int(cat['%08x' % t]['length'] or 0) < 0x400 and n.startswith('??0'):
                found += scan(t, t + int(cat['%08x' % t]['length']), 0)
    return found
out = open(A + '/evidence/rtti.tsv', 'w'); C = collections.Counter()
for f in sorted(glob.glob(A + '/runs/r*/PLAN.tsv')):
    for ln in open(f):
        pp = ln.rstrip('\n').split('\t'); a, ct = pp[0], pp[2]; want = mang(ct)
        sites = sorted(int(c[0], 16) for c in x.get(a, []) if c[1] == 'call'); per = []
        for s in sites:
            fa = [c for c in x[a] if int(c[0], 16) == s][0][2]
            lo = max(s - 0x300, int(fa, 16) if fa else s - 0x300)
            prev = [q for q in sites if q < s and q >= lo]
            if prev: lo = prev[-1] + 5
            fs = scan(lo, s)
            cls = [z.replace('ctor:', '') for z in fs]
            per.append(set(cls))
        hits = [s for s in per if want in s]
        other = [s for s in per if s and want not in s]
        v = 'RTTI_OK' if hits else ('RTTI_DERIVED' if other else 'NONE')
        C[v] += 1
        print(a, ct, want, v, ' | '.join(','.join(sorted(s)) for s in per)[:300], sep='\t', file=out)
print(C)
