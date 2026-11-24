# tri334a.py: NF334 one-line-per-wrong-table-line triage of ENTRIES (T1 other).
# Columns: idx target witness wflags same_method pdb_exact standin_classes wrong_symbol
#  wflags: OFF:<why> / LOCK=<owner> / MINE / NOSRC (no landed row at the witness)
#  same_method: the wrong spelling's method token equals the PDB's
#  pdb_exact: the wrong spelling is itself an exact PDB name-set symbol (egor / fablewin): a wrong-target / overload candidate
#  standin: classes / scopes in the wrong spelling found in neither PDB name set
import sys, re, os
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); S = os.path.dirname(os.path.dirname(A)); R = '/home/user/FableDecomp'
sys.path.insert(0, A + '/tools'); import nf
G = R + '/ghidra_out/'
P = set(); txt = []
for nm in ('egor', 'fablewin'):
    for l in open(G + nm + '_pdb_names.tsv', encoding='utf-8', errors='replace'):
        txt.append(l)
        for f in l.rstrip('\n').split('\t'):
            if f.startswith('?') or f.startswith('_') or f.startswith('@'): P.add(f)
pdbtxt = ''.join(txt)
why = {l.split('\t')[0]: l.split('\t')[1].strip() for l in open(A + '/work/offlimits_why.tsv')}
T = {}
for l in open(R + '/rebuild/oracles/reloc_bindings.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) == 3: T.setdefault(p[1].lower().replace('0x', ''), []).append((p[0], p[2].lower().replace('0x', '')))
def classes(s): return set(re.findall(r'[VU]([A-Za-z_]\w*)@', s)) | set(re.findall(r'@([A-Za-z_]\w*)@@', s))
def meth(s):
    m = re.match(r'\?\?([0-9A-Z_])', s)
    if m: return '??' + m.group(1)
    m = re.match(r'\?(\w+)@', s) or re.match(r'[_@]?(\w+?)(@\d+)?$', s)
    return m.group(1) if m else '?'
def fl(a):
    f = []
    if a in why: f.append('OFF:' + why[a])
    o = S + '/w22/nflocks/%s/owner' % a
    if os.path.exists(o):
        w = open(o).read().strip()
        if w not in ('released', 'NF334'): f.append('LOCK=' + w)
    src = nf.src_of(a)
    if not src or not src.exists(): f.append('NOSRC')
    return ','.join(f) or '-'
E = nf.entries()
idxs = list(map(int, sys.argv[1:])) or range(1, len(E) + 1)
for i in idxs:
    e = E[i - 1]; t = e[0]; pdb = e[6]
    for s, w in T.get(t, []):
        if s == pdb: continue
        bad = sorted(c for c in classes(s) - classes(pdb) if not re.search(r'[VU]%s@|@%s@@' % (re.escape(c), re.escape(c)), pdbtxt))
        print('%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s' % (i, t, w, fl(w), 'Y' if meth(s) == meth(pdb) else 'N', 'Y' if s in P else 'N', ','.join(bad) or '-', s))
