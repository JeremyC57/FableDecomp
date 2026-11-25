#!/usr/bin/env python3
"""objsec440.py <run dir> <ref RD obj>: HEAD v new object per row (obj/H_<a>.obj v obj/<a>.obj); non-debug sections paired by (name, COMDAT
symbol) under the renames {old self -> new self, ReleaseData<CUnprovenPointee_a> -> ReleaseData<T>}: characteristics, bytes, relocations.
Allowed and listed: SH rows' pick-any ReleaseData<T> copy whose body changed to the Data-first _RELEASEDATA body, accepted only when
its bytes + relocations equal the reference ReleaseData row object's leaf (a D53 RD row: retail's RD body). Anything else -> NOT_EQ."""
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf, coff
B, REF = sys.argv[1], sys.argv[2]
P = [l.rstrip('\n').split('\t') for l in open(B + '/PLAN.tsv') if l.strip()]
def secs(path, mp):
    raw = nf.coff(path)[0]; cs = coff.sections(path); d = {}
    for s, c in zip(raw, cs):
        if s['name'].startswith('.debug'): continue
        k = (s['name'], mp.get(c['sym'], c['sym']) if c['sym'] else None)
        d[k] = (s['ch'], s['data'], [(v, mp.get(y, y), t) for v, y, t in s['rel']], c['sel'])
    return d
ref = [v for k, v in secs(REF, {}).items() if k[0] == '.text' and k[1] and k[1].startswith('?ReleaseData@')][0]
out = open(B + '/evidence/objsec.txt', 'w'); n_ok = 0
for p in P:
    a, mem, old, new = p[0], p[1], p[3], p[4]
    Tn = re.search(r'\?\$CCountedPointer@(.+?@@)@', new).group(1)
    rd_old = '?ReleaseData@?$CCountedPointer@VCUnprovenPointee_%s@@@@QAEXXZ' % a
    rd_new = '?ReleaseData@?$CCountedPointer@%s@@QAEXXZ' % Tn
    mp = {old: new, rd_old: rd_new}
    h, n = B + '/obj/H_%s.obj' % a, B + '/obj/%s.obj' % a
    H = secs(h, mp); N = secs(n, {}); bad = []; notes = []
    for k in set(H) | set(N):
        if k in H and k in N:
            if H[k][:3] != N[k][:3]:
                if k[1] == rd_new and mem == 'SH' and N[k][1:3] == ref[1:3] and N[k][0] == ref[0]:
                    notes.append('SH_RD:pick-any %s now the Data-first RD body (= RD row leaf, %dB sel%d)' % (rd_new, len(N[k][1]), N[k][3]))
                else: bad.append('%s/%s differs' % k)
        else: bad.append('%s only in %s' % (k, 'HEAD' if k in H else 'new'))
    eH = {x[:4] + mp.get(x[4:], x[4:]) for x in nf.ext_syms(h)}; eN = nf.ext_syms(n)
    if eH ^ eN: bad.append('ext sets differ: %s' % sorted(eH ^ eN))
    selH = [s['sel'] for s in coff.sections(h) if s['sym'] == old]; selN = [s['sel'] for s in coff.sections(n) if s['sym'] == new]
    v = ('ALL_EQ' + ('+' + ','.join(sorted(notes)) if notes else '')) if not bad else 'NOT_EQ ' + '; '.join(bad[:4])
    n_ok += not bad
    print('%s\t%s\t%s\tsections=%d\tleaf_sel H=%s N=%s\t%s -> %s' % (a, mem, v, len(N), selH, selN, old, new), file=out)
out.close(); print(B, 'ok', n_ok, 'of', len(P))
