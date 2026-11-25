#!/usr/bin/env python3
"""rdcmp428.py: for each form-b (inst) row, its leaf Reset<T> COMDAT and its extra pick-any ?ReleaseData@<T> COMDAT v every census object (work/cen) that already
DEFines the same name: bytes and relocation symbols equal (R1 tier 1 / R10). -> evidence/rdcmp.tsv"""
import sys, os, glob
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf, coff
A = str(nf.A); out = open(A + '/evidence/rdcmp.tsv', 'w')
def sec(path, sym):
    raw = nf.coff(path)[0]; cs = coff.sections(path)
    for s, c in zip(raw, cs):
        if c['sym'] == sym and s['name'].startswith('.text'): return (s['data'], [(v, y, t) for v, y, t in s['rel']])
cen = {}
for cf in ('reset_census.tsv', 'witness_census.tsv'):
    for l in open(A + '/evidence/' + cf):
        f = l.rstrip('\n').split('\t')
        if len(f) == 4 and f[2] == 'DEF' and f[1] not in cen.get(f[3], []): cen.setdefault(f[3], []).append(f[1])
for b in sorted(glob.glob(A + '/runs/r*/PLAN.tsv')):
    B = os.path.dirname(b)
    for l in open(b):
        p = l.rstrip('\n').split('\t')
        if p[6] != 'inst': continue
        T = p[4].split('?$CCountedPointer@')[1].split('@@@@')[0]; rd = '?ReleaseData@?$CCountedPointer@%s@@@@QAEXXZ' % T
        for nm in (p[4], rd):
            mine = sec(B + '/obj/%s.obj' % p[0], nm); res = []
            for a in cen.get(nm, []):
                if a == p[0]: continue
                o = sec(A + '/work/cen/%s.obj' % a, nm)
                res.append('%s:%s' % (a, 'EQ' if o == mine else 'NEQ'))
            print(p[0], nm, len(mine[0]) if mine else '-', ' '.join(res) or 'NEW_TO_CENSUS', sep='\t', file=out)
out.close()
