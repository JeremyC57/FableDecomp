#!/usr/bin/env python3
"""vtcmp425.py <batch> <tag>: vftable slot agreement of the ??_7<C>@@6B@ COMDAT each row's object emits, HEAD v new, against retail.
Per slot: the relocation's symbol -> its retail address (tracker names / landed selves, then reloc_bindings.tsv); agree when it equals the
retail vftable dword (rtti-vtables.tsv VA of class C, read from FABLE_EXE). Writes batches/<b>/evidence/vtcmp.tsv
(address, class, retail_slots, head slots/agree/unknown, new slots/agree/unknown) and prints a summary."""
import sys, os, glob, collections, json
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF427/tools425'); import lib425 as L
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
import csv as _c
EXP = {}
for r_ in _c.DictReader(open('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/ns_now/OPEN.tsv'), delimiter='\t'):
    e_ = r_['rtti'] or r_['pdb_spelling']
    if e_: EXP[r_['target']] = e_
for l_ in open(L.R + '/rebuild/oracles/self_symbols.tsv'):
    f_ = l_.rstrip('\n').split('\t')
    if len(f_) > 1 and f_[1] and f_[0] not in EXP: EXP[f_[0]] = f_[1]
out = []
import bisect
CODE=sorted(int(r[0],16) for r in L.tsv(L.R + "/rebuild/oracles/auto-re-candidates.tsv")[1:] if r and r[0][:1] in "0123456789abcdef")
def retlen(va):
    n=0
    while n<200:
        d=int.from_bytes(pe.read_va(os.environ["FABLE_EXE"], va+4*n, 4),"little")
        if not (0x401000<=d<0x01200000): break
        n+=1
    return n
for a in rows:
    C = L.WL[a]['klass']; vts = sorted({v for v, s, f in VT.get(C, [])})
    if not vts: out.append([a, C, 'NO_RETAIL_VT']); continue
    va = vts[0]; nret = max(s for v, s, f in VT[C] if v == va) + 1
    ret = [int.from_bytes(pe.read_va(os.environ['FABLE_EXE'], va + 4 * i, 4), 'little') for i in range(nret)]
    nreal = retlen(va); ret = [int.from_bytes(pe.read_va(os.environ['FABLE_EXE'], va + 4 * i, 4), 'little') for i in range(nreal)]
    row = [a, C, '0x%08X:%d/%d' % (va, nret, nreal)]
    for k in ('h', 'n'):
        o = glob.glob('%s/work/wd_%s%s/*/*/%s/%s.obj' % (W, tag, k, a, a))
        sl_ = slots(o[0], C) if o else None
        if sl_ is None: row.append('none'); continue
        agree = unk = wrong = uok = ubad = unoexp = 0; wl=[]
        for i, s in enumerate(sl_):
            ad = S2A.get(s)
            if not ad:
                unk += 1
                if k == 'n' and i < len(ret):
                    ex = EXP.get('%08x' % ret[i])
                    if ex is None: wl.append('%d:NOEXP:%s' % (i, s[:60])); unoexp += 1
                    elif ex == s: uok += 1
                    else: ubad += 1; wl.append('%d:MISM:%s!=%s' % (i, s[:70], ex[:70]))
            elif i < len(ret) and ('%08x' % ret[i]) in ad: agree += 1
            else: wrong += 1; wl.append('%d:WRONG:%s' % (i, s[:50]))
        row.append('%d slots %d agree %d unbound [exact %d MISMATCH %d noexp %d] %d WRONG %s' % (len(sl_), agree, unk, uok, ubad, unoexp, wrong, ' '.join(wl[:4])))
    out.append(row)
with open('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/vtx_' + b + '.tsv', 'w') as f:
    f.write('address\tclass\tretail_vftable\thead\tnew\n')
    for x in out: f.write('\t'.join(x) + '\n')

