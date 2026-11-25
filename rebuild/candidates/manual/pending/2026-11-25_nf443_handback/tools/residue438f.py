#!/usr/bin/env python3
"""residue438f.py <accepted batch...>: RESIDUE.tsv for every T1_A target with a wrong spelling the accepted batches do not close.
One line per (target, wrong spelling, caller row) with the generator / precheck / gate reason; a wrong spelling with no located caller
gets row '-'. Also prints the closed / partly / untouched target counts."""
import sys, os, csv, collections, re, json, glob
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); R = '/home/user/FableDecomp'
acc = sys.argv[1:]
op = {r['target']: r for r in csv.DictReader(open(A + '/tracker/OPEN.tsv'), delimiter='\t')}
tab = collections.defaultdict(set)
for r in csv.DictReader(open(R + '/rebuild/oracles/reloc_bindings.tsv'), delimiter='\t'): tab[r['target']].add(r['symbol'])
done = set(); deleted = set()
for b in acc:
    for l in open(A + '/batches/%s/renames.tsv' % b):
        a, s, P, t = l.rstrip('\n').split('\t'); done.add((a, s, t))
    for l in open(A + '/batches/%s/TABLE_CHANGES.tsv' % b).read().splitlines()[1:]:
        f = l.split('\t')
        if f[0] == 'delete': deleted.add((f[1], f[2]))
why = {}
def put(t, s, a, r):
    for tt in t.split(','):
        why.setdefault((tt, s, a), r)
for f in ['work/r4_RESIDUE.tsv', 'work/r1_RESIDUE.tsv']:
    for l in open(A + '/' + f).read().splitlines()[1:]:
        t, s, a, r = l.split('\t')[:4]; put(t, s, a, 'GEN: ' + r)
# planned rows that failed the clang precheck
for tag in ('r4', 'r1'):
    for l in open(A + '/work/%s_PLAN.tsv' % tag):
        a, mode, s, P, t = l.rstrip('\n').split('\t')
        e = A + '/work/pre_err_%sN_%s.txt' % (tag, a)
        if os.path.exists(e): put(t, s, a, 'PRECHECK (generated edit does not compile): ' + re.sub(r'^.*?error: ', '', open(e).readline().strip())[:160])
for h in glob.glob(A + '/batches/*_hold/RESULTS.tsv'):
    b = h.split('/')[-2]; ren = [l.rstrip('\n').split('\t') for l in open(os.path.dirname(h) + '/renames.tsv')]
    res = {l.split('\t')[0]: l.rstrip('\n').split('\t')[4] for l in open(h).read().splitlines()[1:]}
    for a, s, P, t in ren: why[(t, s, a)] = 'HOLD %s: %s' % (b, res.get(a, '')[:200])
cen = collections.defaultdict(set)
for l in open(A + '/work/census_f.tsv'):
    a, t, s, P, n = l.rstrip('\n').split('\t'); cen[(t, s)].add(a)
hits = collections.defaultdict(set)
for l in open(A + '/work/ast.jsonl'):
    j = json.loads(l)
    for x in j.get('hits', []): hits[x['sym']].add(j['row'])
out = []; st = collections.Counter()
for t, r in op.items():
    P = r['pdb_spelling']; wrong = sorted(s for s in tab[t] if s != P and (s, t) not in deleted)
    touched = any(d[2] == t for d in done)
    st['closed' if not wrong else ('partly' if touched else 'open')] += 1
    for s in wrong:
        rows = sorted((hits.get(s, set()) & cen[(t, s)]) - {a for a, ss, tt in done if ss == s and tt == t})
        keys = [k for k in why if k[0] == t and k[1] == s]
        if not rows and not keys:
            out.append((t, s, '-', 'NOLOC: no landed caller located by libclang (implicit call such as a destructor, special member, a row whose parse failed, or a stale table line)')); continue
        for a in sorted(set(rows) | {k[2] for k in keys}):
            out.append((t, s, a, why.get((t, s, a), 'UNCLASSIFIED (located but no generator verdict)')))
with open(A + '/RESIDUE.tsv', 'w') as f:
    f.write('target\twrong_symbol\trow\treason\n'); f.write(''.join('\t'.join(x) + '\n' for x in out))
print(st, 'lines', len(out)); print(collections.Counter(re.split(r'[:(]', x[3])[0] + ':' + (re.split(r': ', x[3])[1][:50] if ': ' in x[3] else '') for x in out).most_common(25))
