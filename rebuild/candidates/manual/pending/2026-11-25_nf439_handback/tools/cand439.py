#!/usr/bin/env python3
"""cand439.py: candidate caller rows for the B-list T1 targets. A row is a candidate for target t when its retail bytes call / jump to t
(E8/E9 rel32) and its landed source names the function-name token of some wrong spelling of t. Writes work/cand.tsv (row, target, tokens)."""
import sys, os, re, csv, glob, struct, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
csv.field_size_limit(10**9)
R = '/home/user/FableDecomp'; W = str(nf.A); S = str(nf.S)
B = {r['target']: r for r in csv.DictReader(open(S + '/t1split/T1_B.tsv'), delimiter='\t')}
files = {}
for f in glob.glob(R + '/rebuild/src/compiled/*/*/*.cpp'):
    m = re.search(r'_([0-9a-f]{8})\.cpp$', f)
    if m: files.setdefault(m.group(1), f)
cat = nf.catalog()
refs = collections.defaultdict(set)
for a in files:
    if a not in cat: continue
    b = bytes.fromhex(cat[a]['bytes']); va = int(a, 16)
    for i in range(len(b) - 4):
        if b[i] in (0xE8, 0xE9):
            t = '%08x' % ((va + i + 5 + struct.unpack_from('<i', b, i + 1)[0]) & 0xffffffff)
            if t in B: refs[t].add(a)
def tokens(s):
    m = re.match(r'^\?\?[0-9A-Z_]\??(\w+)@', s)
    if s.startswith('??0') or s.startswith('??1'): m = re.match(r'^\?\?[01](\w+)@', s)
    elif s.startswith('?'): m = re.match(r'^\?(\w+)@', s)
    else: m = re.match(r'^[_@]?(\w+?)(@\d+)?$', s)
    return m.group(1) if m else None
out = []; n = collections.Counter()
for t, r in B.items():
    P = r['pdb_spelling']; toks = set()
    for sp in r['spellings'].split(' || '):
        m = re.match(r'^(\S+) \(', sp.strip())
        if m and m.group(1) != P:
            k = tokens(m.group(1))
            if k: toks.add(k)
    for a in sorted(refs.get(t, ())):
        x = open(files[a], encoding='utf-8', errors='replace').read()
        hit = sorted(k for k in toks if re.search(r'\b%s\b' % re.escape(k), x))
        if hit: out.append((a, t, ','.join(hit))); n[t] += 1
open(W + '/work/cand.tsv', 'w').write(''.join('\t'.join(x) + '\n' for x in out))
print('pairs', len(out), 'rows', len({x[0] for x in out}), 'targets', len(n)); print(n.most_common(8))
