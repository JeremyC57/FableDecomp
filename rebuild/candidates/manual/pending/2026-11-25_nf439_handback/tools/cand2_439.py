#!/usr/bin/env python3
"""cand2_439.py: narrower candidates: (row, target, wrong spelling) where the row's retail bytes call t, and the row's source declares the
wrong spelling's function locally: scope token defined in the row (class/struct/namespace S) and the name token present, or for a free
spelling a free declaration `Name(` at file level. ctor/dtor spellings: the row defines the stand-in class locally. Writes work/cand2.tsv."""
import sys, os, re, csv, glob, struct, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
csv.field_size_limit(10**9)
R = '/home/user/FableDecomp'; W = str(nf.A); S = str(nf.S)
B = {r['target']: r for r in csv.DictReader(open(S + '/t1split/T1_B.tsv'), delimiter='\t')}
files = {}
for f in glob.glob(R + '/rebuild/src/compiled/*/*/*.cpp'):
    m = re.search(r'_([0-9a-f]{8})\.cpp$', f)
    if m: files.setdefault(m.group(1), f)
cat = nf.catalog(); refs = collections.defaultdict(set)
for a in files:
    if a not in cat: continue
    b = bytes.fromhex(cat[a]['bytes']); va = int(a, 16)
    for i in range(len(b) - 4):
        if b[i] in (0xE8, 0xE9):
            t = '%08x' % ((va + i + 5 + struct.unpack_from('<i', b, i + 1)[0]) & 0xffffffff)
            if t in B: refs[t].add(a)
txt = {}
def T(a):
    if a not in txt: txt[a] = open(files[a], encoding='utf-8', errors='replace').read()
    return txt[a]
def parse(s):
    m = re.match(r'^\?\?([01])(\w+)@((?:[\w$]+@)*)@', s)
    if m: return ('ctor' if m.group(1) == '0' else 'dtor'), m.group(2), [m.group(2)] + [x for x in m.group(3).split('@') if x]
    m = re.match(r'^\?(\w+)@((?:[\w$]+@)*)@', s)
    if m: return 'fn', m.group(1), [x for x in m.group(2).split('@') if x]
    return None
out = []; c = collections.Counter()
for t, r in B.items():
    P = r['pdb_spelling']
    for sp in r['spellings'].split(' || '):
        m = re.match(r'^(\S+) \(', sp.strip())
        if not m or m.group(1) == P: continue
        s = m.group(1); p = parse(s)
        if not p: continue
        kind, name, scope = p
        for a in sorted(refs.get(t, ())):
            x = T(a)
            if not re.search(r'\b%s\b' % re.escape(name), x): continue
            if scope:
                if not re.search(r'\b(class|struct|namespace)\s+%s\b' % re.escape(scope[0]), x): continue
            else:
                if not re.search(r'^[^/\n]*\b%s\s*\([^;{]*\)\s*;' % re.escape(name), x, re.M): continue
            out.append((a, t, s)); c[t] += 1
open(W + '/work/cand2.tsv', 'w').write(''.join('\t'.join(x) + '\n' for x in out))
print('pairs', len(out), 'rows', len({x[0] for x in out}), 'targets', len(c)); print(c.most_common(12))
