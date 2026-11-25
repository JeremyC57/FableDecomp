#!/usr/bin/env python3
"""dispo427.py: one line per family worklist row: where it went (batch) or why it stays open -> evidence/dispo.tsv; T3.txt residue lines."""
import csv, os, glob, collections, re
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); S = os.environ['S']; N5 = S + '/w22/agentNF425/batches'
cl = {r['addr']: r for r in csv.DictReader(open(A + '/evidence/classify.tsv'), delimiter='\t')}
sh = {r['addr']: r for r in csv.DictReader(open(A + '/evidence/shapes.tsv'), delimiter='\t')}
bat = {}
for d in glob.glob(A + '/batches/*'):
    if os.path.exists(d + '/RESULTS.tsv'):
        for r in csv.DictReader(open(d + '/RESULTS.tsv'), delimiter='\t'): bat[r['address']] = (os.path.basename(d), r['verdict'])
    else:
        for a in open(d + '/rows.txt').read().split(): bat[a] = (os.path.basename(d), 'PENDING')
n5 = {}
for d in glob.glob(N5 + '/*'):
    for a in open(d + '/rows.txt').read().split(): n5[a] = os.path.basename(d)
gen = {}
for f in ('gc_all.tsv', 'gj_all.tsv', 'gv_all.tsv', 'gvl_all.tsv', 'gvl_final_hd.tsv'):
    p = A + '/work/' + f
    if os.path.exists(p):
        for r in csv.reader(open(p), delimiter='\t'):
            if r and r[1] in ('HOLD', 'NOFORM'): gen.setdefault(r[0], (f.split('_')[0], r[4] if f.startswith('g' + 'c') or f.startswith('gj') else r[3]))
fl = {}
for p in (A + '/work/gen1/GEN.tsv', A + '/work/dry2/GEN.tsv'):
    for l in open(p).read().splitlines()[1:]:
        q = l.split('\t')
        if q[1] == 'SKIP': fl.setdefault(q[0], q[5])
vth = set(open(S + '/nf425_vtheld.txt').read().split())
out = []; c = collections.Counter()
for a, r in sorted(cl.items()):
    m = r['member']; s = sh[a]['shape']
    if a in bat and bat[a][1] in ('ACCEPT', 'PENDING'): kind, why = 'ACCEPT', 'batch %s' % bat[a][0]
    elif a in bat: kind, why = 'HOLD', 'batch %s: %s' % bat[a]
    elif a in gen: kind, why = 'HOLD', '%s generator: %s' % gen[a]
    elif m == 'Clone' and a in n5 and a not in vth: kind, why = 'NF425', 'NF425 batch %s (pending landing; not this pass)' % n5[a]
    elif m == 'Clone' and a in vth: kind, why = 'HOLD', 'Part 2: %s' % {'00860290': 'NF323 Q3 / NF327 unit with binders 0x0085FF10 / 0x00860150 (T3 held lines); not re-formed', '008edbd0': 'CCreatureAction_Die: one unit with the four rtti_conflict FinishAction rows (0x00865E20 0x0087E840 0x008C3F00 0x008C9FC0)'}.get(a, 'no form (see gen tables)')
    elif m == 'Clone': kind, why = 'RESIDUE', 'Clone row outside NF425 batches (NF425 T3 residue)'
    elif m in ('?0',): kind, why = 'RESIDUE', 'constructor split (%s): callers bind other spellings of the ctor; needs per-class caller respell after the class model lands (not generated this pass)' % r['status']
    elif m in ('?1',): kind, why = 'RESIDUE', 'destructor split (%s): not generated this pass' % r['status']
    elif m in ('?_G', '?_E'): kind, why = 'RESIDUE', 'deleting destructor: NF424 G1 now applies to the 120 classes given a VIRTUAL_MODEL here (queue after the headers land)'
    elif r['status'] == 'rtti_conflict': kind, why = 'RESIDUE', 'rtti_conflict: the RTTI placement names another class\'s member (one unit with its class; identity research)'
    elif r['status'] in ('split_fixable', 'split_unknown', 'standin'): kind, why = 'RESIDUE', '%s %s: caller/self respell not generated this pass (retail shape %s)' % (r['status'], m, s)
    elif a in fl: kind, why = 'RESIDUE', 'flatten_pilot: ' + fl[a]
    else: kind, why = 'RESIDUE', 'virtual override, retail shape %s (len %s): no shape generator this pass' % (s, sh[a]['len'])
    c[kind] += 1
    out.append([a, kind, r['status'], r['class'], m, s, why])
with open(A + '/evidence/dispo.tsv', 'w') as f:
    f.write('address\tdisposition\tstatus\tclass\tmember\tretail_shape\treason\n')
    for x in out: f.write('\t'.join(x) + '\n')
print(c)
