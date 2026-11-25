#!/usr/bin/env python3
"""worklist.py: T1 targets of the NF423 classes -> per caller row worklist (from reloc_bindings.tsv, not truncated OPEN columns).
Writes evidence/targets.tsv and evidence/callers.tsv (caller, target, wrong_symbol, pdb_spelling, klass, fix, lock, t3, claimB)."""
import csv, sys, os, re, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
R = '/home/user/FableDecomp'; A = str(nf.A)
CL = sys.argv[1:] or ['CCountedPointer', 'CThingCreatureBase', 'CThing', 'NCreatureMode', 'CAIStateGroupBase', 'CCreatureActionBase', 'CTCBase']
csv.field_size_limit(10**9)
op = list(csv.DictReader(open(A + '/tracker/OPEN.tsv'), delimiter='\t'))
T = [r for r in op if r['tier'] == 'T1' and r['klass'] in CL]
tab = collections.defaultdict(list)
for r in csv.DictReader(open(R + '/rebuild/oracles/reloc_bindings.tsv'), delimiter='\t'):
    tab[r['target']].append((r['symbol'], r['address']))
t3 = open(R + '/rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv').read().lower()
t3set = set()
for l in open(R + '/rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv').read().splitlines()[1:]:
    q = l.split('\t')
    if len(q) > 2:
        for x in re.findall(r'0x([0-9a-fA-F]{8})', q[1] + ' ' + q[2]): t3set.add(x.lower())
claims = open(R + '/rebuild/candidates/manual/COORDINATION_CLAIMS.tsv').read().lower()
out = open(A + '/evidence/callers.tsv', 'w'); w = csv.writer(out, delimiter='\t', lineterminator='\n')
w.writerow(['caller', 'target', 'wrong_symbol', 'pdb', 'klass', 'fix', 'lock_caller', 'lock_target', 't3_caller', 't3_target', 'src'])
tt = open(A + '/evidence/targets.tsv', 'w')
for r in T:
    a = r['target']; pdb = r['pdb_spelling']
    wrong = [(s, wi) for s, wi in tab[a] if s != pdb]
    tt.write('\t'.join([a, r['klass'], r['fix'], pdb, str(len(wrong)), nf.lock_owner(a), 'T3' if a in t3set else '']) + '\n')
    for s, wi in wrong:
        src = nf.src_of(wi)
        w.writerow([wi, a, s, pdb, r['klass'], r['fix'], nf.lock_owner(wi), nf.lock_owner(a), 'T3' if wi in t3set else '', 'T3' if a in t3set else '', str(src).replace(R + '/rebuild/src/compiled/', '') if src else 'NOSRC'])
