# fam_anong.py <ov> <rows.txt|-> : the c661/c652 file-static CCharString globals (queue rows `?NAME@?A0x<TU hash>@@3VCCharString@@A`):
# every row naming a bound stand-in spelling of the target (the static initializer defines it, same-TU callers extern it) puts it
# in the anonymous namespace under the PDB identifier: `namespace { CCharString NAME(...); }` / `namespace { extern CCharString
# NAME; }`, uses renamed. The ?A0x hash is this lane's TU hash (tools match ignoring it; bound as compiled).
import sys, re, os, json, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib2 as F, inputs
BT, BS = inputs.bindings()
JOBS = collections.defaultdict(list)   # row -> [(target, NAME, oldident, oldsym)]
IXD = {}
for l in open(inputs.W + '/idx/full.jsonl'):
    r = json.loads(l); IXD[r['a']] = set(r.get('u', [])) | set(r.get('d', []))
WHO = collections.defaultdict(set)
for a, s in IXD.items():
    for x in s: WHO[x].add(a)
TG = {}
for q in open(inputs.W + '/QUEUE_W15_input.tsv'):
    f = q.rstrip('\n').split('\t')
    m = re.match(r'^\?(\w+)@\?A0x<[^>]*>@@3VCCharString@@A$', f[2].strip()) if len(f) > 2 else None
    if not m: continue
    t = inputs.na(f[1]); TG[t] = m.group(1)
    for s in BT.get(t, ()):
        mo = re.match(r'^\?(\w+)@@3VCCharString@@A$', s)
        if not mo: continue
        for a in WHO.get(s, ()): JOBS[a].append((t, m.group(1), mo.group(1), s))
def tr(t, a):
    msgs = []
    for tg, NAME, X, s in JOBS[a]:
        c = F.code_only(t)
        if re.search(r'\b' + NAME + r'\b', c): return None, '%s already used here' % NAME
        dm = list(re.finditer(r'(?m)^[ \t]*(extern\s+)?CCharString\s+' + X + r'\s*(\([^;]*\))?\s*;', c))
        if len(dm) != 1: return None, '%d declarations of %s' % (len(dm), X)
        d = dm[0]; stmt = t[d.start():d.end()]
        stmt2 = re.sub(r'\b' + X + r'\b', NAME, stmt.lstrip())
        ind = stmt[:len(stmt) - len(stmt.lstrip())]
        t = t[:d.start()] + ind + 'namespace { ' + stmt2 + ' }' + t[d.end():]
        t = F.rename_code(t, X, NAME)
        t = re.sub(r'(?m)^(//.*)\b' + X + r'\b', lambda mm: mm.group(0).replace(X, "`anonymous namespace'::" + NAME), t)
        t = t.replace("the object's name is descriptive, its\n// real identifier is not recoverable.", "the object is the PDB's file-static\n// `anonymous namespace'::%s (naming wave 15)." % NAME)
        t = t.replace("the object's name is descriptive, its real identifier is not recoverable.", "the object is the PDB's file-static `anonymous namespace'::%s (naming wave 15)." % NAME)
        msgs.append('%s -> anon %s' % (X, NAME))
    return t, ' '.join(msgs)
def ck(D0, U0, D1, U1, a):
    S0 = D0 | U0; S1 = D1 | U1
    for tg, NAME, X, s in JOBS[a]:
        if s in S1: return 'old still named'
        if not any(re.match(r'^\?' + NAME + r'@\?A0x[0-9a-f]{8}@@3VCCharString@@A$', x) for x in S1): return 'new not named'
    olds = {j[3] for j in JOBS[a]}
    rem = [x for x in S0 - S1 if x not in olds and '?A0x' not in x and not x.startswith('??__')]
    add = [x for x in S1 - S0 if '?A0x' not in x and not x.startswith('??__')]
    if rem or add: return 'diff -%s +%s' % (rem[:3], add[:3])
    return None
if __name__ == '__main__':
    rows = sorted(JOBS) if sys.argv[2] == '-' else [l.split()[0] for l in open(sys.argv[2]) if l.strip()]
    F.run(sys.argv[1], rows, tr, ck, 'anong')
