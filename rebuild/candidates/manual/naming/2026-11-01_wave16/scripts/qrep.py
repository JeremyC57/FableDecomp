# qrep.py: per queue row -- target, final, rows naming a non-final bound spelling (u), whether the row itself is landed and
# defines the final spelling, and whether the final is bound / named anywhere
import sys, os, json, collections, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs
from rows import rows
R = rows()
BT, BS = inputs.bindings()
U = collections.defaultdict(set); D = collections.defaultdict(set); ROWD = {}
for l in open(inputs.W + '/idx/full.jsonl'):
    r = json.loads(l)
    for s in r.get('u', []): U[s].add(r['a'])
    for s in r.get('d', []): D[s].add(r['a'])
    ROWD[r['a']] = set(r.get('d', []))
for i, l in enumerate(open(inputs.W + '/QUEUE_W16_input.tsv')):
    q = l.rstrip('\n').split('\t') + [''] * 5
    ch, a, fin = q[0], q[1], q[2].strip()
    t = inputs.na(a) if re.match(r'^(0x)?[0-9a-fA-F]{6,8}$', a.strip()) else a
    alts = [s for s in BT.get(t, ()) if s != fin]
    ru = set()
    for s in alts: ru |= U.get(s, set())
    own = t in R
    owndef = fin in ROWD.get(t, set())
    print('%d\t%s\t%s\t%s\tu=%d\town=%s%s\tfinU=%d finB=%s\t%s' % (i, ch, t, fin[:90], len(ru), 'Y' if own else 'n', '+fin' if owndef else '', len(U.get(fin, ())), 'Y' if fin in BS else 'n', ' '.join(sorted(ru)[:12])))
