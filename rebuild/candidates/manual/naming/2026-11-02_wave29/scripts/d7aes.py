# d7aes.py: the CActiveEntityScript<T> D7 rows (shape 0x48:~CScriptThing 0x34:~CCountedPointer 0x00CE1000 0x0:~CSpawnedFuncBase): T from
# (1) the storer CEntityScriptBinding<T>::CreateActiveEntityScript row's own source (the CActiveEntityScript<..> it constructs / its Leaf) and
# (2) slot 4's SuspendableProcess row Leaf; (3) PDB: ?SuspendableProcess@?$CActiveEntityScript@V<T>@NScript@@@@UAEXXZ and the ??_G / ??_E.
import sys, os, re, ast, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pe
from rows import rows, ROOT
R = rows()
PDB = collections.defaultdict(set)
for tag, f in (('E', 'egor_pdb_names.tsv'), ('F', 'fablewin_pdb_names.tsv')):
    for l in open('/home/user/FableDecomp/ghidra_out/' + f, errors='replace'):
        q = l.rstrip('\n').split('\t')
        if len(q) >= 2 and 'CActiveEntityScript@' in q[1]: PDB[q[1]].add(tag)
def src(a):
    d = R.get(a); return open(os.path.join(ROOT, 'rebuild/src/compiled', d['Source']), encoding='utf-8', errors='replace').read() if d else ''
def tnames(t):
    s = set(m.group(1) for m in re.finditer(r'\?\$C(?:EntityScriptBinding|ActiveEntityScript)@[VU](C\w+)@NScript@', t))
    s |= set(m.group(1) for m in re.finditer(r'CActiveEntityScript<\s*(?:NScript::)?(C\w+)\s*>', t))
    s |= set(m.group(1) for m in re.finditer(r'CEntityScriptBinding<\s*(?:NScript::)?(C\w+)\s*>', t))
    return s
SH = {l.split()[1]: l.split()[3:] for l in open('runs/d7shape.txt')}
out = []
for l in open('runs/d7map.tsv'):
    f = l.rstrip('\n').split('\t'); a = f[1]
    sh = ' '.join(SH[a][:3])
    if sh != '0x48:4aa840 0x34:ce1000 0x0:cdd4c0': continue
    va, rt, st, bd = ast.literal_eval(f[4])[0]
    vt = int(va, 16); s4 = '%08x' % pe.dw(vt + 16)
    T1 = set().union(*[tnames(src(s)) for s in st if not s.startswith('?')]) if st else set()
    T2 = tnames(src(s4))
    T = (T1 | T2)
    ok = len(T) == 1 and (not T1 or not T2 or T1 == T2)
    t = next(iter(T)) if len(T) == 1 else ''
    sp = '?SuspendableProcess@?$CActiveEntityScript@V%s@NScript@@@@UAEXXZ' % t
    g = '??_G?$CActiveEntityScript@V%s@NScript@@@@UAEPAXI@Z' % t; e = '??_E?$CActiveEntityScript@V%s@NScript@@@@UAEPAXI@Z' % t
    out.append((a, va, ','.join(st), s4, ' '.join(sorted(T1)), ' '.join(sorted(T2)), t if ok else '', ''.join(sorted(PDB.get(sp, ''))), ''.join(sorted(PDB.get(g, ''))) + '/' + ''.join(sorted(PDB.get(e, '')))))
with open('runs/d7aes.tsv', 'w') as o:
    o.write('row\tvtable\tstorers\tslot4\tT_storer\tT_slot4\tT\tPDB_SuspendableProcess\tPDB_G/E\n')
    for r in out: o.write('\t'.join(r) + '\n')
print(len(out), collections.Counter((bool(r[6]), r[7], bool(r[4]), bool(r[5])) for r in out))
