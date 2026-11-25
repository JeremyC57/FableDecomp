#!/usr/bin/env python3
"""trial_gen.py: for each remaining CLONE_INLINE local-class row (batches i1..i4), every polymorphic branch engine/<C>.h offers (each
#ifdef FABLE_RETAIL_* block, and the default branch) that defines `class C : public ...` and declares Clone: compile the real member on it
(the row's prelude + that opt-in + the block's 'Needs' defines) and report leaf / extra DEFs / ??_7 retail-slot agreement v the local
form. Also records, per header, why no branch fits. Out: work/trialg/trial.tsv."""
import sys, os, re, glob, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib425 as L
import pe
W = L.A; G = W + '/work/trialg'; os.makedirs(G, exist_ok=True)
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
            return sum(1 for k, s in enumerate(rel) if k < nret and ('%08x' % ret[k]) in S2A.get(s, ()))
    return None
def blocks(ht):
    """(name, text) per top-level-ish #ifdef FABLE_RETAIL_X ... matching #else/#endif, plus ('', default text after #else of the guard)."""
    out = []
    for m in re.finditer(r'^#ifdef (FABLE_RETAIL_\w+)\s*$', ht, re.M):
        depth = 0; i = m.end(); end = len(ht)
        for d in re.finditer(r'^#\s*(if|ifdef|ifndef|else|elif|endif)\b', ht[m.end():], re.M):
            k = d.group(1)
            if k in ('if', 'ifdef', 'ifndef'): depth += 1
            elif k == 'endif':
                if depth == 0: end = m.end() + d.start(); break
                depth -= 1
            elif depth == 0: end = m.end() + d.start(); break
        out.append((m.group(1), ht[m.end():end]))
    return out
out = []
for b in ('i1', 'i2', 'i3', 'i4'):
    for a in open('%s/batches/%s/rows.txt' % (W, b)).read().split():
        C = L.WL[a]['klass']; h = L.R + '/rebuild/include/engine/%s.h' % C
        if not os.path.exists(h): out.append([b, a, C, 'NOHDR']); continue
        ht = open(h).read(); rt = open('%s/batches/%s/rows/%s.cpp' % (W, b, a)).read()
        cands = [(n, t) for n, t in blocks(ht) if re.search(r'class %s\s*:\s*public' % re.escape(C), t)]
        if not cands and re.search(r'class %s\s*:\s*public' % re.escape(C), re.sub(r'#ifdef FABLE_RETAIL_\w+.*?#endif[^\n]*', '', ht, flags=re.S)):
            cands = [('', ht)]
        if not cands: out.append([b, a, C, 'NO_POLY_BRANCH']); continue
        leaf = re.search(r'// Leaf:\s*(\S+)', rt).group(1); rett = re.search(r'@@[UEM]BEPAV(\w+)@@XZ', leaf).group(1)
        lines = rt.splitlines(); defs = [l for l in lines if l.startswith('#define ')]; incs = [l for l in lines if l.startswith('#include ')]
        mine = glob.glob('%s/work/wd_%sn/*/*/%s/%s.obj' % (W, b, a, a)); vl = agree(mine[0], C) if mine else None
        for n, t in cands:
            if not re.search(r'\bClone\s*\(', t): out.append([b, a, C, 'BRANCH_NO_CLONE', n]); continue
            needs = re.findall(r'FABLE_RETAIL_\w+', (re.search(r'// Needs[^\n]*', t) or re.search('', '')).group(0))
            d2 = defs + ['#define ' + x for x in ([n] if n else []) + needs if '#define ' + x not in defs]
            src = '\n'.join(d2 + incs + ['#include "engine/%s.h"' % C]) + '\n\n%s* %s::Clone() const\n{\n    return new %s(*this);\n}\n' % (rett, C, C)
            f = G + '/%s_%s.cpp' % (a, n or 'default'); open(f, 'w').write(src)
            rc, o = L.compile_(f, f[:-4] + '.obj')
            if rc: out.append([b, a, C, 'NOCOMPILE', n, ' '.join(re.findall(r'error C\d+', o)[:2])]); continue
            dfn, und = L.syms(f[:-4] + '.obj'); lc = L.leafcmp(f[:-4] + '.obj', leaf, a)
            extra = [s for s in dfn if s not in (leaf, '??_7%s@@6B@' % C, '??0%s@@QAE@ABV0@@Z' % C, '??_G%s@@UAEPAXI@Z' % C)]
            out.append([b, a, C, 'TRY', n, 'leaf=%s' % lc, 'extra=%d' % len(extra), 'vt=%s local=%s' % (agree(f[:-4] + '.obj', C), vl)])
with open(G + '/trial.tsv', 'w') as fo:
    for x in out: fo.write('\t'.join(map(str, x)) + '\n')
print(collections.Counter(x[3] + ('' if x[3] != 'TRY' else ' ' + x[5]) for x in out))
for x in out:
    if x[3] == 'TRY' and x[5] == 'leaf=EQ': print('\t'.join(map(str, x))[:220])
