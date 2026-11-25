#!/usr/bin/env python3
"""vtcmp425.py <batch> <tag>: vftable slot agreement of the ??_7<C>@@6B@ COMDAT each row's object emits, HEAD v new, against retail.
Per slot: the relocation's symbol -> its retail address (tracker names / landed selves, then reloc_bindings.tsv); agree when it equals the
retail vftable dword (rtti-vtables.tsv VA of class C, read from FABLE_EXE). Writes batches/<b>/evidence/vtcmp.tsv
(address, class, retail_slots, head slots/agree/unknown, new slots/agree/unknown) and prints a summary."""
import sys, os, glob, collections, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib425 as L
import pe
W = L.A; b, tag = sys.argv[1:3]; B = W + '/batches/' + b
rows = open(B + '/rows.txt').read().split()
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
out = []
for a in rows:
    C = L.WL[a]['klass']; vts = sorted({v for v, s, f in VT.get(C, [])})
    if not vts: out.append([a, C, 'NO_RETAIL_VT']); continue
    va = vts[0]; nret = max(s for v, s, f in VT[C] if v == va) + 1
    ret = [int.from_bytes(pe.read_va(os.environ['FABLE_EXE'], va + 4 * i, 4), 'little') for i in range(nret)]
    row = [a, C, '0x%08X:%d' % (va, nret)]
    for k in ('h', 'n'):
        o = glob.glob('%s/work/wd_%s%s/*/*/%s/%s.obj' % (W, tag, k, a, a))
        sl_ = slots(o[0], C) if o else None
        if sl_ is None: row.append('none'); continue
        agree = unk = 0
        for i, s in enumerate(sl_):
            ad = S2A.get(s)
            if not ad: unk += 1
            elif i < nret and ('%08x' % ret[i]) in ad: agree += 1
        row.append('%d/%d agree, %d unbound' % (agree, len(sl_), unk))
    out.append(row)
with open(B + '/evidence/vtcmp.tsv', 'w') as f:
    f.write('address\tclass\tretail_vftable\thead\tnew\n')
    for x in out: f.write('\t'.join(x) + '\n')
for x in out: print('\t'.join(x))
