# qout.py <before idx> <after idx> <out.tsv>: per queue row (QUEUE_W17_input.tsv order) -- rows naming a wrong spelling of the
# target before / after the wave (wrong = the target's bound spellings other than the final + the mangled names listed in the
# queue's wrong_spellings_seen column), own-row status before / after, and which rows changed.
import sys, os, json, collections, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs
from rows import rows
R = rows()
BT, BS = inputs.bindings()
def load(p):
    U = collections.defaultdict(set); D = {}
    for l in open(p):
        r = json.loads(l)
        if 'err' in r: continue
        for s in r.get('u', []) + r.get('d', []): U[s].add(r['a'])
        D[r['a']] = set(r.get('d', []))
    return U, D
UB, DB = load(sys.argv[1]); UA, DA = load(sys.argv[2])
out = open(sys.argv[3], 'w')
out.write('i\tchunk\ttarget\tfinal\twrong_rows_before\twrong_rows_after\town\town_fin_before\town_fin_after\tremaining\n')
for i, l in enumerate(open(inputs.W + '/QUEUE_W17_input.tsv')):
    q = l.rstrip('\n').split('\t') + [''] * 5
    ch, a, fin, ev, ws = q[0], q[1], q[2].strip(), q[3], q[4]
    t = inputs.na(a) if re.match(r'^(0x)?[0-9a-fA-F]{6,8}$', a.strip()) else a
    wrong = set(s for s in BT.get(t, ()) if s != fin)
    wrong |= set(m for m in re.findall(r'[?@][^\s;|,()]+', ws) if m != fin and len(m) > 4)
    norm = lambda x: re.sub(r'\?A0x[0-9a-f]{8}', '?A0x', x)
    if '?A0x' in fin: wrong = set(w for w in wrong if norm(w) != norm(fin))
    rb = set(); ra = set()
    for s in wrong: rb |= UB.get(s, set()); ra |= UA.get(s, set())
    own = t in R
    ofb = own and (fin in DB.get(t, set()) or ('?A0x' in fin and any(norm(x) == norm(fin) for x in DB.get(t, ()))))
    ofa = own and (fin in DA.get(t, set()) or ('?A0x' in fin and any(norm(x) == norm(fin) for x in DA.get(t, ()))))
    out.write('%d\t%s\t%s\t%s\t%d\t%d\t%s\t%s\t%s\t%s\n' % (i, ch, t, fin, len(rb), len(ra), 'Y' if own else 'n', ofb, ofa, ' '.join(sorted(ra))))
