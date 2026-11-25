#!/usr/bin/env python3
"""tablefix443.py <batch>: decide every delete / rewitness line of a finalized batch from COMPILED HEAD objects (tools439/census439.py,
DB work/census_db.tsv). For each ACCEPT rename (old, t) whose (old, t) table line exists: candidates = landed rows (not the batch's ACCEPT rows
that rename (old, t)) whose retail bytes reference t (rel32 call/jmp or imm32) and whose source names old's identifier; each candidate's HEAD
object is compiled and its leaf relocations resolved against retail. No verified binder -> `delete` (witness = a same-batch ACCEPT row that
renames it) in TABLE_CHANGES.tsv. Verified binders and the current witness renamed here -> `rewitness` to the first binder, written to
CROSSBATCH.tsv (its witness is outside the batch). Adds stay in TABLE_CHANGES.tsv. Writes evidence/tablefix.txt."""
import sys, os, re, csv, glob, struct, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
W = str(nf.A); R = '/home/user/FableDecomp'
b = sys.argv[1]; B = W + '/batches/' + b
acc = {r['address'] for r in csv.DictReader(open(B + '/RESULTS.tsv'), delimiter='\t') if r['verdict'] == 'ACCEPT'}
ren = [r for r in csv.reader(open(B + '/renames.tsv'), delimiter='\t') if r and r[0] in acc]
tab = {}
for r in csv.DictReader(open(os.environ.get('TABLE', R + '/rebuild/oracles/reloc_bindings.tsv')), delimiter='\t'): tab.setdefault((r['symbol'], r['target']), r['address'])
DB = W + '/work/census_db.tsv'
def db():
    d = set(); rows = set(); bad = set()
    if os.path.exists(DB):
        for l in open(DB):
            r = l.rstrip('\n').split('\t')
            if len(r) > 3 and r[1] == '-': bad.add(r[0]); continue
            rows.add(r[0])
            if len(r) > 3: d.add((r[0], r[1], r[2]))
    return d, rows - bad
files = {}
for f in glob.glob(R + '/rebuild/src/compiled/*/*/*.cpp'):
    m = re.search(r'_([0-9a-f]{8})\.cpp$', f)
    if m: files.setdefault(m.group(1), f)
cat = nf.catalog()
def refs(a, t):
    if a not in cat: return False
    bb = bytes.fromhex(cat[a]['bytes']); va = int(a, 16)
    if any(bb[i] in (0xE8, 0xE9) and '%08x' % ((va + i + 5 + struct.unpack_from('<i', bb, i + 1)[0]) & 0xffffffff) == t for i in range(len(bb) - 4)): return True
    return any('%08x' % struct.unpack_from('<I', bb, i)[0] == t for i in range(len(bb) - 3))
def ident(s):
    m = re.match(r'^\?\?[0-1]\??\$?(\w+)@', s) or re.match(r'^\?(\w+)@', s) or re.match(r'^[_@]?(\w+?)(@\d+)?$', s)
    return m.group(1) if m else None
_txt = {}
def names(f, w):
    if f not in _txt: _txt[f] = set(re.findall(r'\w+', open(f, encoding='utf-8', errors='replace').read())) if os.path.exists(f) else set()
    return w in _txt[f]
tc = [l.rstrip('\n').split('\t') for l in open(B + '/TABLE_CHANGES.tsv')][1:]
adds = [x for x in tc if x[0] == 'add']
dels, rew, log = [], [], []
for old, t in sorted({(r[1], r[3]) for r in ren}):
    if (old, t) not in tab: continue
    movers = sorted(r[0] for r in ren if r[1] == old and r[3] == t)
    n = ident(old)
    cand = sorted(x for x, f in files.items() if x not in movers and x != t and n and names(f, n) and refs(x, t))
    d, done = db(); todo = [x for x in cand if x not in done]
    if todo:
        open(W + '/work/tf_rows_%s.txt' % b, 'w').write('\n'.join(todo) + '\n')
        subprocess.run([sys.executable, W + '/tools439/census439.py', W + '/work/tf_rows_%s.txt' % b, DB], capture_output=True)
        d, done = db()
    ver = [x for x in cand if (x, t, old) in d]
    unk = [x for x in cand if x not in done]
    if not ver and not unk:
        dels.append(['delete', old, t, movers[0], 'NF443: no landed row outside this batch binds the stand-in at this target after the respell (compiled HEAD census of %d candidate row(s) naming the identifier and calling the target)' % len(cand)])
        act = 'delete'
    elif ver and tab[(old, t)] in movers:
        rew.append(['rewitness', old, t, ver[0], 'NF443: the witness %s moved to the PDB spelling in batch %s; %d landed row(s) still bind the stand-in here (compiled HEAD census: %s)' % (tab[(old, t)], b, len(ver), ' '.join(ver[:5]))])
        act = 'rewitness (CROSSBATCH)'
    elif unk:
        act = 'KEEP: %d candidate(s) not compiled (%s); table line left as is' % (len(unk), ' '.join(unk[:5]))
        if tab[(old, t)] in movers: act = 'PROBLEM: witness moved but candidates uncompiled ' + ' '.join(unk[:5])
    else: act = 'keep (witness %s not in batch; %d binder(s))' % (tab[(old, t)], len(ver))
    log.append('%s\t%s\tmovers=%s\tcandidates=%d\tverified=%s\t-> %s' % (old, t, ','.join(movers), len(cand), ','.join(ver[:5]), act))
H = ['action', 'symbol', 'target', 'witness', 'note']
open(B + '/TABLE_CHANGES.tsv', 'w').write('\t'.join(H) + '\n' + ''.join('\t'.join(x) + '\n' for x in adds + dels))
open(B + '/CROSSBATCH.tsv', 'w').write('\t'.join(H) + '\n' + ''.join('\t'.join(x) + '\n' for x in rew))
os.makedirs(B + '/evidence', exist_ok=True); open(B + '/evidence/tablefix.txt', 'w').write('\n'.join(log) + '\n')
print(b, 'add', len(adds), 'delete', len(dels), 'rewitness(cross)', len(rew)); print('\n'.join(l for l in log if 'PROBLEM' in l or 'KEEP' in l))
