#!/usr/bin/env python3
"""linkprep440.py <run dir>: /FORCE link sets for a run: HEAD = obj/H_<a>.obj, new = obj/<a>.obj, each plus the same definers:
NF428's census objects (151 landed objects naming CCountedPointer, read-only) that DEFine one of the run's new names (leaf or SH's RD copy),
and the HEAD objects of the landed rows whose relocations bind one of those names (reloc_bindings witness column; compiled here).
Writes work/link_<run>.args and evidence/definers.tsv (name, definer, body EQ/NEQ v this run's copy)."""
import sys, os, glob, re, shutil, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf, coff
from lib440 import *
B = sys.argv[1]; run = os.path.basename(B.rstrip('/'))
P = [l.rstrip('\n').split('\t') for l in open(B + '/PLAN.tsv') if l.strip()]
names = {}
for p in P:
    names[p[4]] = p[0]
    if p[1] == 'SH':
        Tn = re.search(r'\?\$CCountedPointer@(.+?@@)@', p[4]).group(1); names['?ReleaseData@?$CCountedPointer@%s@@QAEXXZ' % Tn] = p[0]
def body(obj, sym):
    raw = nf.coff(obj)[0]; cs = coff.sections(obj)
    for s, c in zip(raw, cs):
        if c['sym'] == sym and s['name'].startswith('.text'): return (s['data'], [(v, y, t) for v, y, t in s['rel']])
cen = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF428/work/cen'
defs = set(); rows = []
for o in sorted(glob.glob(cen + '/*.obj')):
    for s in nf.ext_syms(o):
        k, n = s.split(' ', 1)
        if k == 'DEF' and n in names:
            defs.add(o); mine = body(B + '/obj/%s.obj' % names[n], n); theirs = body(o, n)
            rows.append([n, 'census:' + os.path.basename(o)[:8], 'EQ' if mine == theirs else 'NEQ'])
s2a, a2s, selfs = bindings(); wit = set()
for l in tsv(P('rebuild/oracles/reloc_bindings.tsv')) if False else tsv(R + '/rebuild/oracles/reloc_bindings.tsv'):
    if len(l) > 2 and l[0] in names: wit.add(norm(l[2]))
os.makedirs(W + '/work/wit', exist_ok=True); wobj = []
for w in sorted(wit):
    # the witness address is inside a landed row: find the row whose catalog range covers it
    cat, keys = catalog(); import bisect
    i = bisect.bisect(keys, w) - 1; a = keys[i] if i >= 0 else None
    if not a or not nf.src_of(a): continue
    dst = W + '/work/wit/%s.obj' % a
    if not os.path.exists(dst):
        try:
            st, leaf, fl, obj, wd = nf.compile_one(a, nf.src_of(a), 'wit')
            if obj.exists(): shutil.copy(obj, dst)
        except Exception as e: print('witness', a, 'ERR', e); continue
    if os.path.exists(dst):
        wobj.append(dst)
        for s in nf.ext_syms(dst):
            k, n = s.split(' ', 1)
            if k == 'DEF' and n in names:
                rows.append([n, 'witness:' + a, 'EQ' if body(B + '/obj/%s.obj' % names[n], n) == body(dst, n) else 'NEQ'])
extra = sorted(defs) + sorted(set(wobj))
H = [B + '/obj/H_%s.obj' % p[0] for p in P] + extra; N = [B + '/obj/%s.obj' % p[0] for p in P] + extra
open(W + '/work/link_%s.args' % run, 'w').write(' '.join(H) + ' -- ' + ' '.join(N))
open(B + '/evidence/definers.tsv', 'w').write('name\tdefiner\tbody_vs_row\n' + ''.join('\t'.join(r) + '\n' for r in rows))
print(run, 'definers', len(defs), 'witness objs', len(set(wobj)), 'definer lines', len(rows), collections.Counter(r[2] for r in rows))
