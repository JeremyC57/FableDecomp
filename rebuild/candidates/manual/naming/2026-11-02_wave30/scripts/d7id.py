# d7id.py: per D7 row -- shape, vtable, the storers' Leafs, slot 4's landed row Leaf, and the T each names; writes runs/d7id.tsv
import sys, os, re, ast, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pe
from rows import rows, ROOT
R = rows()
def leaf(a):
    d = R.get(a)
    if not d: return None
    t = open(os.path.join(ROOT, 'rebuild/src/compiled', d['Source']), encoding='utf-8', errors='replace').read()
    m = re.search(r'^// Leaf: (.*)$', t, re.M); return m.group(1).strip() if m else None
SH = {}
for l in open(os.path.join(sys.argv[1])):
    f = l.split(None, 4); SH[f[1]] = f[3:4] and l.split()[3:-0]
out = []
for l in open('runs/d7map.tsv'):
    f = l.rstrip('\n').split('\t'); a = f[1]; vts = ast.literal_eval(f[4])
    va, rt, st, bd = vts[0]
    vt = int(va, 16); s4 = '%08x' % pe.dw(vt + 16)
    sl = [(s, leaf(s)) for s in st if not s.startswith('?')]
    TS = set()
    for s, lf in sl + [(s4, leaf(s4))]:
        if lf:
            for m in re.finditer(r'\?\$C(?:EntityScriptBinding|ActiveEntityScript|SpawnedFunc1?)@V(\w+)@(NScript)?@?', lf): TS.add(m.group(1) + ('@NScript' if m.group(2) else ''))
            m = re.search(r'@(C\w+)@NScript@@', lf)
            if m and not TS: TS.add(m.group(1) + '@NScript?')
    out.append((f[0], a, va, ' '.join('%s=%s' % (s, lf) for s, lf in sl)[:300], '%s=%s' % (s4, leaf(s4)), ' '.join(sorted(TS))))
with open('runs/d7id.tsv', 'w') as o:
    for r in out: o.write('\t'.join(r) + '\n')
print(collections.Counter(len(r[5].split()) for r in out))
