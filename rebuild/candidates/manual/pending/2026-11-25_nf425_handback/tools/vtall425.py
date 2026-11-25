#!/usr/bin/env python3
"""vtall425.py: every landed row of another family that can emit ??_7<C>@@6B@ for a class this hand-back's CLONE_INLINE rows emit
(work/vt_cand.tsv: constructor / deleting-destructor / Clone / ??_7-naming rows, compiled HEAD into work/g/<a>_D) v the NF425 row's own
object: retail-slot agreement of each emitted ??_7, and the slots the landed copy has right that the NF425 copy gets wrong.
Writes evidence/vtall.tsv and prints a per-batch verdict count (NF425_WORSE = some landed copy names a retail slot this one misses)."""
import sys, os, re, glob, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib425 as L
import pe
W = L.A
S2A = collections.defaultdict(set)
for t, r in L.REC.items():
    for n in r.get('names', []): S2A[n].add(t)
    if r.get('self_leaf'): S2A[r['self_leaf']].add(t)
for s, v in L.TABW.items():
    for t, w in v: S2A[s].add(t)
VT = collections.defaultdict(list)
for r in L.tsv(L.R + '/rebuild/manifest/rtti-vtables.tsv')[1:]: VT[r[0]].append((int(r[1], 16), int(r[2]), int(r[3], 16)))
RET = {}
def ret(C):
    if C not in RET:
        va = sorted({v for v, s, f in VT.get(C, [])})[0]; nret = max(s for v, s, f in VT[C] if v == va) + 1
        RET[C] = [int.from_bytes(pe.read_va(os.environ['FABLE_EXE'], va + 4 * i, 4), 'little') for i in range(nret)]
    return RET[C]
def ok(obj, C):
    if not obj or not os.path.exists(obj): return None
    secs, sl = L.nf.coff(obj)
    for i, nm, v, sec, t, scl in sl:
        if nm == '??_7%s@@6B@' % C and sec > 0:
            rel = [s for o, s, ty in sorted(secs[sec - 1]['rel'])]; r = ret(C)
            return [k < len(r) and ('%08x' % r[k]) in S2A.get(s, ()) for k, s in enumerate(rel)]
    return None
where = {}
for b in ('i1', 'i2', 'i3', 'i4', 'h1', 'h2', 'h3'):
    for a in open('%s/batches/%s/rows.txt' % (W, b)).read().split(): where[a] = b
cand = collections.defaultdict(list)
for l in list(open(W + '/work/vt_cand.tsv')) + list(open(W + '/work/vt_cand2.tsv')):
    g, C, b, a, bn = l.rstrip('\n').split('\t')
    if a in where: cand[a].append((g, bn))
for g in open(W + '/work/nf424_g_definers.txt').read().split(): pass
out = []; cnt = collections.Counter()
for a, b in sorted(where.items()):
    C = L.WL[a]['klass']
    o = glob.glob('%s/work/wd_%sn/*/*/%s/%s.obj' % (W, b, a, a)); mine = ok(o[0] if o else None, C)
    if mine is None: out.append([b, a, C, 'NO_NF425_VT', '-', '-']); cnt[(b, 'NO_NF425_VT')] += 1; continue
    worst = []; info = []
    for g, bn in cand.get(a, []):
        oth = ok('%s/work/g/%s_D/%s.obj' % (W, g, g), C)
        if oth is None: continue
        lost = [k for k, (x, y) in enumerate(zip(oth, mine)) if x and not y] + [k for k in range(len(mine), len(oth)) if oth[k]]
        info.append('%s:%d/%d%s' % (g, sum(oth), len(oth), (' lost=%s' % lost) if lost else ''))
        if lost: worst.append(g)
    v = 'NO_OTHER_EMITTER' if not info else ('NF425_WORSE' if worst else 'NF425_EQ_OR_BETTER')
    cnt[(b, v)] += 1
    out.append([b, a, C, v, '%d/%d' % (sum(mine), len(mine)), ' '.join(info)])
with open(W + '/evidence/vtall.tsv', 'w') as f:
    f.write('batch\taddress\tclass\tverdict\tnf425_agree\tother_emitters(addr:agree/slots lost=[slots it has right and NF425 misses])\n')
    for x in out: f.write('\t'.join(x) + '\n')
for k in sorted(cnt): print(k, cnt[k])
for x in out:
    if x[3] == 'NF425_WORSE': print('\t'.join(x)[:240])
