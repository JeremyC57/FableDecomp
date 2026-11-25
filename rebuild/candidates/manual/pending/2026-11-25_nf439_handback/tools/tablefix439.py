#!/usr/bin/env python3
"""tablefix439.py <batch> [moved batches...]: verify TABLE_CHANGES rewitness lines with compiled HEAD objects (census439 DB work/census_db.tsv).
For each rewitness (old, t): token candidates (unmoved landed rows whose retail calls t and whose source names old's name and scope tokens)
are compiled (census439) when not yet in the DB; the line becomes a rewitness to the first row whose HEAD object binds old at t, or a
delete when none does. Rewrites TABLE_CHANGES.tsv (keeps the token census note) and writes evidence/tablefix.txt."""
import sys, os, re, csv, glob, struct, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
W = str(nf.A); R = '/home/user/FableDecomp'
b = sys.argv[1]; B = W + '/batches/' + b; moved = set()
for x in (sys.argv[2:] or [b]): moved |= set(open(W + '/batches/%s/rows.txt' % x).read().split())
DB = W + '/work/census_db.tsv'
def db():
    d = set(); rows = set()
    for f in [DB] + glob.glob(W + '/work/census_0*.tsv'):
        if os.path.exists(f):
            for l in open(f):
                r = l.rstrip('\n').split('\t'); rows.add(r[0])
                if len(r) > 3: d.add((r[0], r[1], r[2]))
    return d, rows
files = {}
for f in glob.glob(R + '/rebuild/src/compiled/*/*/*.cpp'):
    m = re.search(r'_([0-9a-f]{8})\.cpp$', f)
    if m: files[m.group(1)] = f
cat = nf.catalog()
def refs(a, t):
    if a not in cat: return False
    bb = bytes.fromhex(cat[a]['bytes']); va = int(a, 16)
    return any(bb[i] in (0xE8, 0xE9) and '%08x' % ((va + i + 5 + struct.unpack_from('<i', bb, i + 1)[0]) & 0xffffffff) == t for i in range(len(bb) - 4))
def toks(sym):
    m = re.match(r'^\?\?[01](\w+)@((?:[\w$]+@)*)@', sym) or re.match(r'^\?(\w+)@((?:[\w$]+@)*)@', sym)
    return (m.group(1), [x for x in m.group(2).split('@') if x]) if m else (None, [])
L = list(csv.reader(open(B + '/TABLE_CHANGES.tsv'), delimiter='\t')); out = [L[0]]; log = []
for r in L[1:]:
    if r[0] != 'rewitness': out.append(r); continue
    old, t = r[1], r[2]; nm, sc = toks(old)
    left = [x for x, f in files.items() if x not in moved and nm and refs(x, t)
            and all(re.search(r'\b%s\b' % re.escape(w), open(f, errors='replace').read()) for w in [nm] + sc[:1])]
    d, done = db(); todo = [x for x in left if x not in done]
    if todo:
        open(W + '/work/tf_rows.txt', 'w').write('\n'.join(todo) + '\n')
        subprocess.run([sys.executable, W + '/tools/census439.py', W + '/work/tf_rows.txt', DB], capture_output=True)
        d, done = db()
    ver = [x for x in left if (x, t, old) in d]
    if ver: out.append(['rewitness', old, t, ver[0], 'NF439: the witness moved to the PDB spelling; %d unmoved landed row(s) still bind the stand-in here (compiled HEAD census: %s)' % (len(ver), ' '.join(ver[:5]))])
    else: out.append(['delete', old, t, r[3], 'NF439: no unmoved landed row binds the stand-in at this target after the respell (compiled HEAD census of %d token candidates)' % len(left)])
    log.append('%s %s candidates=%d verified=%s -> %s' % (old, t, len(left), ver, out[-1][0]))
open(B + '/TABLE_CHANGES.tsv', 'w').write(''.join('\t'.join(x) + '\n' for x in out))
os.makedirs(B + '/evidence', exist_ok=True); open(B + '/evidence/tablefix.txt', 'w').write('\n'.join(log) + '\n'); print('\n'.join(log))
