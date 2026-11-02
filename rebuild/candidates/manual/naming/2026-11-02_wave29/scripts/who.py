# who.py <target>...: per retail target, every bound spelling and the HEAD-index rows naming it (u/d)
import sys, os, json, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs
BT, BS = inputs.bindings()
IX = os.environ.get('IX', os.path.join(inputs.W, 'idx/full.jsonl'))
E = collections.defaultdict(set)
for l in open(IX):
    r = json.loads(l)
    for s in r.get('u', []): E[s].add(r['a'])
    for s in r.get('d', []): E[s].add(r['a'] + '*')
for t in sys.argv[1:]:
    t = inputs.na(t)
    print('==', t)
    for s in sorted(BT.get(t, ())):
        print('  %-90s %s' % (s, ' '.join(sorted(E.get(s, ())))))
