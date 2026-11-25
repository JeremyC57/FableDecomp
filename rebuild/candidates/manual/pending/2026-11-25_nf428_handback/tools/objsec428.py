#!/usr/bin/env python3
"""objsec428.py <batch dir>: HEAD v new object per Reset row (obj/H_<a>.obj v obj/<a>.obj); non-debug sections paired by (name, COMDAT
symbol) under the renames {old self -> new self, ??0SCountBlock@@QAE@PAX@Z -> ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z}: characteristics,
bytes, relocations. Allowed and listed, not hidden: R2 (NF371 ruling, landed nf371_b2) the unreferenced pick-any count-block ctor copy
(stand-in 1-arg ctor -> header 3-arg ctor, both 25 B, bodies differ); form b (inst) the extra pick-any ?ReleaseData@<T> copy.
Anything else -> NOT_EQ. Output <batch>/evidence/objsec.txt"""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf, coff
B = sys.argv[1]
P = [l.rstrip('\n').split('\t') for l in open(B + '/PLAN.tsv') if l.strip()]
CT_H, CT_N = '??0SCountBlock@@QAE@PAX@Z', '??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z'
def secs(path, mp):
    raw = nf.coff(path)[0]; cs = coff.sections(path); d = {}
    for s, c in zip(raw, cs):
        if s['name'].startswith('.debug'): continue
        k = (s['name'], mp.get(c['sym'], c['sym']) if c['sym'] else None)
        d[k] = (s['ch'], s['data'], [(v, mp.get(y, y), t) for v, y, t in s['rel']], c['sel'])
    return d
out = open(B + '/evidence/objsec.txt', 'w'); n_ok = 0
for p in P:
    a, old, new, form = p[0], p[3], p[4], p[6]
    T = new.split('?$CCountedPointer@')[1].split('@@@@')[0]
    rd = '?ReleaseData@?$CCountedPointer@%s@@@@QAEXXZ' % T
    mp = {old: new, CT_H: CT_N}
    h, n = B + '/obj/H_%s.obj' % a, B + '/obj/%s.obj' % a
    H = secs(h, mp); N = secs(n, {})
    bad = []; notes = []
    for k in set(H) | set(N):
        if k in H and k in N:
            if H[k][:3] != N[k][:3]:
                if k[1] == CT_N and len(H[k][1]) == len(N[k][1]): notes.append('R2:%s %dB body differs' % (CT_N, len(N[k][1])))
                else: bad.append('%s/%s differs' % k)
        elif k in N and k[1] == rd and form == 'inst' and N[k][3] == 2: notes.append('PICKANY:%s %dB sel2' % (rd, len(N[k][1])))
        elif k in N and k[1] == CT_N and N[k][3] == 2 and CT_H not in [x[1] for x in H]: notes.append('NEWCTOR:%s %dB' % (CT_N, len(N[k][1])))
        else: bad.append('%s only in %s' % (k, 'HEAD' if k in H else 'new'))
    eH = {x[:4] + mp.get(x[4:], x[4:]) for x in nf.ext_syms(h)}; eN = nf.ext_syms(n)
    dx = [x for x in sorted(eH ^ eN) if x[4:] not in (rd,)]
    if dx: bad.append('ext sets differ: %s' % dx)
    selH = [s['sel'] for s in coff.sections(h) if s['sym'] == old]; selN = [s['sel'] for s in coff.sections(n) if s['sym'] == new]
    v = ('ALL_EQ' + ('+' + ','.join(sorted(notes)) if notes else '')) if not bad else 'NOT_EQ ' + '; '.join(bad[:4])
    n_ok += not bad
    print('%s\t%s\t%s\tsections=%d\tleaf_sel H=%s N=%s\t%s -> %s' % (a, form, v, len(N), selH, selN, old, new), file=out)
out.close(); print(B, 'ok', n_ok, 'of', len(P))
