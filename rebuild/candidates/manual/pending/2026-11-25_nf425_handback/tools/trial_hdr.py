#!/usr/bin/env python3
"""trial_hdr.py: for the CLONE_INLINE rows whose engine/<C>.h has a VIRTUAL_MODEL opt-in, try the shared-header form: the row's own
#define / #include prelude (minus its local class) + the model opt-in + its 'Needs' defines + #include "engine/<C>.h", body
`return new C(*this);`. Reports leaf EQ, extra DEFs and ??_7 slot agreement with retail (v the local-class row). work/trialh/."""
import sys, os, re, glob, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib425 as L
import pe
W = L.A; G = W + '/work/trialh'; os.makedirs(G, exist_ok=True)
S2A = collections.defaultdict(set)
for t, r in L.REC.items():
    for n in r.get('names', []): S2A[n].add(t)
    if r.get('self_leaf'): S2A[r['self_leaf']].add(t)
for s, v in L.TABW.items():
    for t, w in v: S2A[s].add(t)
VT = collections.defaultdict(list)
for r in L.tsv(L.R + '/rebuild/manifest/rtti-vtables.tsv')[1:]: VT[r[0]].append((int(r[1], 16), int(r[2]), int(r[3], 16)))
def agree(obj, C):
    va = sorted({v for v, s, f in VT.get(C, [])})[0]; nret = max(s for v, s, f in VT[C] if v == va) + 1
    ret = [int.from_bytes(pe.read_va(os.environ['FABLE_EXE'], va + 4 * i, 4), 'little') for i in range(nret)]
    secs, sl = L.nf.coff(obj)
    for i, nm, v, sec, t, scl in sl:
        if nm == '??_7%s@@6B@' % C and sec > 0:
            rel = [s for o, s, ty in sorted(secs[sec - 1]['rel'])]
            return sum(1 for k, s in enumerate(rel) if k < nret and ('%08x' % ret[k]) in S2A.get(s, ())), len(rel), nret
    return None
sel = sys.argv[1:]
out = []
for b in ('i1', 'i2', 'i3', 'i4'):
    for a in open('%s/batches/%s/rows.txt' % (W, b)).read().split():
        if sel and a not in sel: continue
        C = L.WL[a]['klass']; h = L.R + '/rebuild/include/engine/%s.h' % C
        if not os.path.exists(h): continue
        ht = open(h).read(); m = re.search(r'#ifdef (FABLE_RETAIL_\w+_VIRTUAL_MODEL)', ht)
        if not m: continue
        needs = re.findall(r'FABLE_RETAIL_\w+', (re.search(r'// Needs[^\n]*', ht[m.end():]) or re.search('', '')).group(0))
        rt = open('%s/batches/%s/rows/%s.cpp' % (W, b, a)).read()
        pre = [l for l in rt.splitlines() if l.startswith('#define ') or l.startswith('#include ')]
        defs = [l for l in pre if l.startswith('#define ')]; incs = [l for l in pre if l.startswith('#include ')]
        for n in [m.group(1)] + needs:
            if '#define ' + n not in defs: defs.append('#define ' + n)
        leaf = re.search(r'// Leaf:\s*(\S+)', rt).group(1)
        rett = re.search(r'@@[UEM]BEPAV(\w+)@@XZ', leaf).group(1)
        src = '\n'.join(defs + incs + ['#include "engine/%s.h"' % C]) + '\n\n%s* %s::Clone() const\n{\n    return new %s(*this);\n}\n' % (rett, C, C)
        f = G + '/%s.cpp' % a; open(f, 'w').write(src)
        rc, o = L.compile_(f, f[:-4] + '.obj'); open(f[:-4] + '.log', 'w').write(o)
        if rc: out.append([b, a, C, 'NOCOMPILE', ' '.join(re.findall(r'error C\d+[^\n]{0,80}', o)[:2])]); continue
        dfn, und = L.syms(f[:-4] + '.obj'); lc = L.leafcmp(f[:-4] + '.obj', leaf, a)
        extra = [s for s in dfn if s not in (leaf, '??_7%s@@6B@' % C, '??0%s@@QAE@ABV0@@Z' % C, '??_G%s@@UAEPAXI@Z' % C)]
        mine = glob.glob('%s/work/wd_%sn/*/*/%s/%s.obj' % (W, b, a, a))
        out.append([b, a, C, 'leaf=%s' % lc, 'extra=%s' % extra, 'vt_hdr=%s' % (agree(f[:-4] + '.obj', C),), 'vt_local=%s' % (agree(mine[0], C) if mine else None,)])
with open(G + '/trial.tsv', 'w') as fo:
    for x in out: fo.write('\t'.join(map(str, x)) + '\n')
for x in out: print('\t'.join(map(str, x))[:300])
print(collections.Counter(x[3] for x in out))
