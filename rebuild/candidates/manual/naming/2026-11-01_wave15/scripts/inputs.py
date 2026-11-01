# wave-13 inputs: queue, PDB name sets, bindings, index
import csv, collections, os, json
ROOT = '/home/user/FableDecomp'
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
def na(a): return a.strip().lower().replace('0x', '').zfill(8)
def queue():
    Q = []
    for i, l in enumerate(open(W + '/QUEUE_W15.tsv')):
        if i == 0: continue
        q = l.rstrip('\n').split('\t') + [''] * 5
        Q.append(q[:5])
    return Q
_N = None
def names():
    global _N
    if _N: return _N
    S = {}
    for tag, p in (('Ego_r.pdb', ROOT + '/ghidra_out/egor_pdb_names.tsv'), ('FableWin.pdb', ROOT + '/ghidra_out/fablewin_pdb_names.tsv'), ('Ego_d.pdb', ROOT + '/ghidra_out/egod_pdb_names.tsv')):
        s = set()
        for l in open(p, encoding='utf-8', errors='replace'):
            q = l.rstrip('\n').split('\t')
            if len(q) >= 2: s.add(q[1])
        S[tag] = s
    _N = S
    return S
def bindings(path=ROOT + '/rebuild/oracles/reloc_bindings.tsv'):
    by_t = collections.defaultdict(set); by_s = {}
    for r in csv.reader(open(path), delimiter='\t', quoting=csv.QUOTE_NONE):
        if len(r) >= 3 and r[0] != 'symbol': by_t[r[1]].add(r[0]); by_s.setdefault(r[0], (r[1], r[2]))
    return by_t, by_s
def index(p=W + '/idx/full.jsonl'):
    E = collections.defaultdict(set); ROWS = {}
    for l in open(p):
        r = json.loads(l); ROWS[r['a']] = r
        for s in r.get('u', []): E[s].add(r['a'])
        for s in r.get('d', []): E[s].add(r['a'])
    return E, ROWS
