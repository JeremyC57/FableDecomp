#!/usr/bin/env python3
"""trial_oj.py: CLONE_OOL (o1/o2) and COPY_JMP F3 (j1..j3) rows whose engine/<C>.h has a polymorphic branch declaring the needed members
(Clone + C(const C&), or Copy + operator=): compile the real member on each such branch (row prelude + opt-in + Needs) and report leaf / DEFs."""
import sys, os, re, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib425 as L
import pe
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
W = L.A; G = W + '/work/trialoj'; os.makedirs(G, exist_ok=True)
out = []
def run(b, a, kind):
    C = L.WL[a]['klass']; h = L.R + '/rebuild/include/engine/%s.h' % C
    if not os.path.exists(h): return
    ht = open(h).read(); rt = open('%s/batches/%s/rows/%s.cpp' % (W, b, a)).read()
    leaf = re.search(r'// Leaf:\s*(\S+)', rt).group(1)
    need = [r'\bClone\s*\(', r'\b%s\s*\(\s*const\s+%s\s*&' % (C, C)] if kind == 'o' else [r'\bCopy\s*\(', r'operator\s*=\s*\(']
    cands = [(n, t) for n, t in blocks(ht) if re.search(r'class %s\s*:\s*public' % re.escape(C), t) and all(re.search(x, t) for x in need)]
    if not cands and all(re.search(x, ht) for x in need) and re.search(r'class %s\s*:\s*public' % re.escape(C), ht): cands = [('', ht)]
    lines = rt.splitlines(); defs = [l for l in lines if l.startswith('#define ')]; incs = [l for l in lines if l.startswith('#include ')]
    if kind == 'o': rett = re.search(r'@@[UEM]BEPAV(\w+)@@XZ', leaf).group(1); body = '%s* %s::Clone() const\n{\n    return new %s(*this);\n}\n' % (rett, C, C)
    else: body = 'void %s::Copy(const CDefClassBase* other)\n{\n    *this = *(const %s*)other;\n}\n' % (C, C)
    for n, t in cands:
        needs = re.findall(r'FABLE_RETAIL_\w+', (re.search(r'// Needs[^\n]*', t) or re.search('', '')).group(0))
        d2 = defs + ['#define ' + x for x in ([n] if n else []) + needs if '#define ' + x not in defs]
        src = '\n'.join(d2 + incs + ['#include "engine/%s.h"' % C]) + '\n\n' + body
        f = G + '/%s_%s.cpp' % (a, n or 'default'); open(f, 'w').write(src)
        rc, o = L.compile_(f, f[:-4] + '.obj')
        if rc: out.append([b, a, C, n, 'NOCOMPILE', ' '.join(re.findall(r'error C\d+', o)[:2])]); continue
        dfn, und = L.syms(f[:-4] + '.obj')
        out.append([b, a, C, n, 'leaf=%s' % L.leafcmp(f[:-4] + '.obj', leaf, a), 'defs=%s' % [s for s in dfn if s != leaf]])
for b in ('o1', 'o2'):
    for a in open('%s/batches/%s/rows.txt' % (W, b)).read().split(): run(b, a, 'o')
for b in ('j1', 'j2', 'j3'):
    for l in list(open('%s/batches/%s/CHECKS.tsv' % (W, b)))[1:]:
        f = l.rstrip('\n').split('\t')
        if 'SHARED_HEADER_AVAILABLE' in f[-1]: run(b, f[0], 'j')
with open(G + '/trial.tsv', 'w') as fo:
    for x in out: fo.write('\t'.join(map(str, x)) + '\n')
for x in out: print('\t'.join(map(str, x))[:230])
