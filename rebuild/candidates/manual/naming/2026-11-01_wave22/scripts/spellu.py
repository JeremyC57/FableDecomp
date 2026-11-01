# spellu.py <target> <final> : non-final bound spellings of target; rows that name it UNDEFINED (u) and rows that only define it (d)
import sys, os, json, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs
t, fin = inputs.na(sys.argv[1]), sys.argv[2]
BT, BS = inputs.bindings()
U = collections.defaultdict(set); D = collections.defaultdict(set)
sy = BT.get(t, set())
for l in open(inputs.W + '/idx/full.jsonl'):
    r = json.loads(l)
    for s in r.get('u', []):
        if s in sy: U[s].add(r['a'])
    for s in r.get('d', []):
        if s in sy: D[s].add(r['a'])
tot = set(); totd = set()
print('final', fin, 'u', len(U.get(fin, ())), 'd', len(D.get(fin, ())))
for s in sorted(sy, key=lambda s: -len(U.get(s, ()))):
    if s == fin: continue
    tot |= U.get(s, set()); totd |= D.get(s, set())
    print('%3d u %3d d  %s  %s %s' % (len(U.get(s, ())), len(D.get(s, ())), s[:140], ' '.join(sorted(U.get(s, ()))[:30]), ('| d: ' + ' '.join(sorted(D.get(s, ()))[:8])) if D.get(s) else ''))
print('rows_u', len(tot), 'rows_d_only', len(totd - tot))
if len(sys.argv) > 3: open(sys.argv[3], 'w').write(''.join(a + '\n' for a in sorted(tot)))
