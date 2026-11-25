#!/usr/bin/env python3
"""pred440.py: per retail D / SH copy (evidence/occ428.json), candidate pointee types T from the functions around it.
VC7.1 /O2 emits an inline's out-of-line copy just before the first COMDAT of the TU that expands it (GOTCHAS nf426), so the
function after a run of copies ("expander") and, less so, the one before are read. Sources per copy:
  NEXT_LAYOUT: CCountedPointer<X> in the Ego_r layout of the expander's class (members, incl. containers of CCountedPointer<X>)
  NEXT_NAME:   CCountedPointer<X> spelled in the expander's own mangled name
  PREV_*:      the same for the function before the run
  NB_MEMBER:   a named CCountedPointer<X> member function adjacent to the run (NF428's loo predictor)
Leave-one-out on the copies that already carry a PDB name -> evidence/pred_loo.tsv; candidates for all -> evidence/pred.json"""
import sys, re, json, collections
sys.path.insert(0, __import__('os').path.dirname(__file__))
from lib440 import *

cat, keys = catalog(); r = rec(); occ = json.load(open(W + '/evidence/occ428.json'))
pl = pool(); lay = layouts()
cpp2M = {}
for M in pl:
    c = demangle_T(M)
    if c: cpp2M.setdefault(c, M)

def nm(a):
    e = r.get(a, {})
    if e.get('status') in ('agree_pdb', 'split_fixable', 'split_pdb_multi') and e.get('pdb_spelling'): return e['pdb_spelling']
    return ''

def Ts_of_name(n):
    out = set()
    for m in re.finditer(r'\?\$CCountedPointer@(V[\w@]+?@@|U[\w@]+?@@|\$\$CBV[\w@]+?@@)@', n or ''):
        if m.group(1) in pl: out.add(m.group(1))
    return out

def Ts_of_class(c):
    out = set()
    for off, typ, mem in lay.get(c, ()):
        for m in CP_RX.finditer(typ):
            M = cpp2M.get(m.group(1))
            if M: out.add(M)
    return out

def mT(n):
    x = member_T(n) if n else None
    return x

idx = {a: i for i, a in enumerate(keys)}
res = {}
for a in sorted(occ):
    if a not in idx: continue
    i = idx[a]
    j = i + 1
    while j < len(keys) and keys[j] in occ: j += 1
    k = i - 1
    while k >= 0 and keys[k] in occ: k -= 1
    nxt = keys[j] if j < len(keys) else None; prv = keys[k] if k >= 0 else None
    src = collections.defaultdict(set)
    for tag, f in (('NEXT', nxt), ('PREV', prv)):
        n = nm(f) if f else ''
        if not n: continue
        x = mT(n)
        if x: src['NB_MEMBER'].add(x[1]); continue
        sc = scope_of(n)
        if sc: src[tag + '_LAYOUT'] |= Ts_of_class(sc)
        src[tag + '_NAME'] |= Ts_of_name(n)
    res[a] = dict(shape=occ[a][0], status=r.get(a, {}).get('status', ''), truth=nm(a), next=nxt, next_name=nm(nxt) if nxt else '',
                  prev=prv, prev_name=nm(prv) if prv else '', run=j - i, src={k: sorted(v) for k, v in src.items() if v})
json.dump(res, open(W + '/evidence/pred.json', 'w'), indent=0)

# leave-one-out on named copies
C = collections.Counter(); rows = []
for a, v in res.items():
    t = mT(v['truth'])
    if not t: continue
    for s, Ts in v['src'].items():
        Ts = set(Ts)
        k = 'uniq_ok' if Ts == {t[1]} else ('uniq_wrong' if len(Ts) == 1 else ('multi_has' if t[1] in Ts else 'multi_miss'))
        C[(s, k)] += 1
    rows.append([a, v['truth'], json.dumps(v['src'])])
    C[('ANY', 'n')] += 1
for k in sorted(C): print(k, C[k])
open(W + '/evidence/pred_loo.tsv', 'w').write(''.join('\t'.join(x) + '\n' for x in rows))
# coverage over open copies
cov = collections.Counter()
for a, v in res.items():
    if 'Unproven' not in (r.get(a, {}).get('self_leaf') or ''): continue
    ks = tuple(sorted(s for s in v['src']))
    cov[ks] += 1
print('coverage of stand-in copies:')
for k, n in cov.most_common(): print(n, k)
