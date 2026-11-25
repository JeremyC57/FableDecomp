#!/usr/bin/env python3
"""vt424cmp.py: for each class with a landed NF424 ??_G row (work/nf424_g_definers.txt), slot agreement with retail of the ??_7<C>@@6B@
the NF424 object emits (work/g/<addr>_D) v the one the NF425 CLONE_INLINE row object emits (work/wd_<b>n). Also records whether the
NF424 source includes engine/<C>.h and whether it declares Clone. Writes evidence/vt424cmp.tsv."""
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
def slots(obj, C):
    secs, sl = L.nf.coff(obj)
    for i, nm, v, sec, t, scl in sl:
        if nm == '??_7%s@@6B@' % C and sec > 0:
            return [s for o, s, ty in sorted(secs[sec - 1]['rel'])]
    return None
def agree(sl_, ret):
    if sl_ is None: return None
    ok = []
    for i, s in enumerate(sl_):
        ad = S2A.get(s); ok.append(bool(ad) and i < len(ret) and ('%08x' % ret[i]) in ad)
    return ok
mine = {}
for b in ('i1', 'i2', 'i3', 'i4'):
    for a in open('%s/batches/%s/rows.txt' % (W, b)).read().split(): mine[L.WL[a]['klass']] = (b, a)
out = []; cnt = collections.Counter()
for g in open(W + '/work/nf424_g_definers.txt').read().split():
    src = L.nf.src_of(g); t = open(src).read()
    m = re.search(r'// Leaf:\s*\?\?_G(\w+)@@', t); C = m.group(1)
    inc = ('engine/%s.h' % C) in t; clone = bool(re.search(r'\bClone\s*\(', t))
    hdr = L.R + '/rebuild/include/engine/%s.h' % C; hclone = os.path.exists(hdr) and bool(re.search(r'\bClone\s*\(', open(hdr).read()))
    va = sorted({v for v, s, f in VT.get(C, [])})[0]; nret = max(s for v, s, f in VT[C] if v == va) + 1
    ret = [int.from_bytes(pe.read_va(os.environ['FABLE_EXE'], va + 4 * i, 4), 'little') for i in range(nret)]
    a424 = agree(slots(glob.glob('%s/work/g/%s_D/%s.obj' % (W, g, g))[0], C), ret)
    b, a = mine.get(C, ('-', None))
    o = glob.glob('%s/work/wd_%sn/*/*/%s/%s.obj' % (W, b, a, a)) if a else []
    a425 = agree(slots(o[0], C), ret) if o else None
    if a424 is None or a425 is None: v = 'MISSING'
    else:
        lost = [i for i, (x, y) in enumerate(zip(a424, a425)) if x and not y]
        v = 'NF425_WORSE' if lost or sum(a425) < sum(a424) else ('EQUAL' if sum(a425) == sum(a424) else 'NF425_BETTER')
        v += (' lost=%s' % lost) if lost else ''
    cnt[v.split()[0]] += 1
    out.append([g, C, b, a or '-', 'inc_hdr=%s' % inc, 'src_clone=%s' % clone, 'hdr_clone=%s' % hclone,
                '%s/%d' % (sum(a424) if a424 else '-', len(a424) if a424 else 0), '%s/%d' % (sum(a425) if a425 else '-', len(a425) if a425 else 0), str(nret), v])
with open(W + '/evidence/vt424cmp.tsv', 'w') as f:
    f.write('nf424_row\tclass\tbatch\tnf425_row\tnf424_includes_header\tnf424_src_mentions_Clone\theader_declares_Clone\tnf424_agree\tnf425_agree\tretail_slots\tverdict\n')
    for x in out: f.write('\t'.join(x) + '\n')
for x in out: print('\t'.join(x))
print(cnt)
