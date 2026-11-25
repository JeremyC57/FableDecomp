#!/usr/bin/env python3
"""gen_cloneinlg.py <addr=OPTIN|addr=> ...: gen_cloneinlh.py for an explicit header branch (from trial_gen.py: the polymorphic branch of
engine/<C>.h that declares Clone; empty OPTIN = the default branch). Out: work/gcg/, work/gcg_final.tsv.
Based on gen_cloneinlh.py: CLONE_INLINE on the shared header. For each CLONE_INLINE row (batches i1..i4) whose engine/<C>.h carries a
FABLE_RETAIL_<C>_VIRTUAL_MODEL opt-in (NF329: the polymorphic retail class, declarations only, retail size, slot order probed against
retail, Clone declared), the row becomes the out-of-class definition of the header's Clone:
    <prelude defines> + <MODEL> + its 'Needs' defines, the prelude includes, #include "engine/<C>.h"
    CCreatureActionBase* C::Clone() const { return new C(*this); }
Checks per row: compiles, leaf EQ retail (relocations masked), DEFs only leaf / ??_7C / ??0C / ??_GC, ??0 base copy ctor + ??2 in UND,
and the emitted ??_7C agrees with retail on at least as many slots as the local-class row's. The test is the i-batch test with its
/alternatename list rebuilt from this object's UND set. Out: work/gch/<a>.cpp, <a>_test.cpp, work/gch_final.tsv (OK / NOFORM rows)."""
import sys, os, re, glob, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib425 as L
import pe
W = L.A; G = W + '/work/gcg'; os.makedirs(G, exist_ok=True)
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
            return sum(1 for k, s in enumerate(rel) if k < nret and ('%08x' % ret[k]) in S2A.get(s, ())), len(rel), nret, va
    return None
BCTOR = {'CCombatActionBase': ('??0CCombatActionBase@@QAE@ABV0@@Z', 0x0062F3B0), 'CCreatureActionBase': ('??0CCreatureActionBase@@QAE@ABV0@@Z', 0x0062EBC0)}
PICK = dict(x.split('=', 1) for x in sys.argv[1:])
res = []
for b in ('i1', 'i2', 'i3', 'i4'):
    for a in open('%s/batches/%s/rows.txt' % (W, b)).read().split():
        C = L.WL[a]['klass']; h = L.R + '/rebuild/include/engine/%s.h' % C
        if not os.path.exists(h): continue
        if a not in PICK: continue
        ht = open(h).read(); MODEL = PICK[a]
        m = re.search(r'^#ifdef %s\s*$' % MODEL, ht, re.M) if MODEL else None
        blk = ht[m.end():] if m else ht
        hc = re.search(r'^// [^\n]*opt-in[^\n]*', blk, re.M | re.I); hc = hc.group(0)[3:] if (hc and MODEL) else ''
        needs = re.findall(r'FABLE_RETAIL_\w+', (re.search(r'// Needs[^\n]*', blk) or re.search('', '')).group(0))
        optin = re.search(r'// Opt-in \(([^)]*)\)', blk); optin = optin.group(1) if optin else '?'
        basem = re.search(r'class %s\s*:\s*public\s+(\w+)' % re.escape(C), blk); base = basem.group(1) if basem else '?'
        rt = open('%s/batches/%s/rows/%s.cpp' % (W, b, a)).read(); tt = open('%s/batches/%s/tests/%s_test.cpp' % (W, b, a)).read()
        lines = rt.splitlines()
        defs = [l for l in lines if l.startswith('#define ')]; incs = [l for l in lines if l.startswith('#include ')]
        added = [n for n in ([MODEL] if MODEL else []) + needs if '#define ' + n not in defs]
        defs += ['#define ' + n for n in added]
        leaf = re.search(r'// Leaf:\s*(\S+)', rt).group(1)
        rett = re.search(r'@@[UEM]BEPAV(\w+)@@XZ', leaf).group(1)
        body = '\n'.join(defs + incs + ['#include "engine/%s.h"' % C]) + '\n\n%s* %s::Clone() const\n{\n    return new %s(*this);\n}\n' % (rett, C, C)
        f = G + '/%s_try.cpp' % a; open(f, 'w').write(body)
        rc, o = L.compile_(f, f[:-4] + '.obj'); open(f[:-4] + '.log', 'w').write(o)
        if rc: res.append([a, 'NOFORM', base, 'rc=%d %s' % (rc, ' '.join(re.findall(r'error C\d+', o)[:3])), b, '']); continue
        dfn, und = L.syms(f[:-4] + '.obj'); lc = L.leafcmp(f[:-4] + '.obj', leaf, a)
        extra = [s for s in dfn if s not in (leaf, '??_7%s@@6B@' % C, '??0%s@@QAE@ABV0@@Z' % C, '??_G%s@@UAEPAXI@Z' % C)]
        bc, bca = BCTOR.get(base, ('?', 0))
        mine = glob.glob('%s/work/wd_%sn/*/*/%s/%s.obj' % (W, b, a, a))
        vh = agree(f[:-4] + '.obj', C); vl = agree(mine[0], C) if mine else None
        if lc != 'EQ' or extra or bc not in und or '??2@YAPAXI@Z' not in und or not vh or (vl and vh[0] < vl[0]):
            res.append([a, 'NOFORM', base, 'leaf=%s extra=%s und_base=%s vt_hdr=%s vt_local=%s' % (lc, extra, bc in und, vh, vl), b, '']); continue
        # header comment: keep the local row's lines up to (not including) '// Header:', then the header form note
        hd = []
        for l in lines:
            if l.startswith('// Header:') or l.startswith('// Members:') or l.startswith('#'): break
            hd.append(l)
        hd = [l.replace(', own members)', ', own members)') for l in hd]
        hd += L.wrap('// Header:', 'engine/%s.h, %s: the polymorphic class there declares this Clone%s; prelude: the CCombatActionBase MELEE block of the landed 0x008CAA70%s. The emitted ??_7%s@@6B@ names %d of retail\'s %d slots right.'
                       % (C, ('opt-in %s' % MODEL) if MODEL else 'its default branch', (' (header: "%s")' % hc.strip()) if hc else '', ' plus that opt-in' if MODEL else '', C, vh[0], vh[2]))
        src = '\n'.join(hd) + '\n' + body
        open(G + '/%s.cpp' % a, 'w').write(src)
        # test: same checks; /alternatename list from this object's UND set
        alt = ['#pragma comment(linker, "/alternatename:%s=_nf425_unused_slot")' % s for s in und if s not in ('??2@YAPAXI@Z', bc)]
        tl = [l for l in tt.splitlines() if 'alternatename:' not in l]
        k = next(i for i, l in enumerate(tl) if l.startswith('extern "C" void nf425_unused_slot'))
        tl = tl[:k + 1] + alt + tl[k + 1:]
        open(G + '/%s_test.cpp' % a, 'w').write('\n'.join(tl) + '\n')
        ev = 'self %s: RTTI placement + %s ; base copy constructor %s 0x%08X (%s; MELEE_COPYCTOR / COPYCTOR opt-in) ; class model engine/%s.h %s' % (
            leaf, '/'.join(L.pdb_src(leaf)) or 'NO PDB', bc, bca, '/'.join(L.pdb_src(bc)) or 'NO PDB', C, MODEL or 'default branch')
        res.append([a, 'OK', base, ev, b, 'engine/%s.h (%s)' % (C, MODEL or 'default branch')])
with open(W + '/work/gcg_final.tsv', 'w') as fo:
    for x in res: fo.write('\t'.join(x) + '\n')
for x in res: print('\t'.join(x)[:260])
print(collections.Counter(x[1] for x in res))
