# the wave-9 inputs: queue proposals (chunks 566-574 + the misfiled chunk-520 row), wave-8 open decisions, renames
import csv, collections, os
ROOT = '/home/user/FableDecomp'
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
W8 = ROOT + '/rebuild/candidates/manual/naming/2026-10-29_wave8'
def na(a): return a.strip().lower().replace('0x', '').zfill(8)
def queue():
    P = collections.defaultdict(list)
    for l in open(W + '/QUEUE_W9.tsv'):
        q = l.rstrip('\n').split('\t')
        if len(q) >= 3: P[na(q[1])].append(('chunk' + q[0], q[2], q[3] if len(q) > 3 else '', q[4] if len(q) > 4 else ''))
    return P
def w8():
    D = {}
    for r in csv.DictReader(open(W8 + '/DECISIONS_W8.tsv'), delimiter='\t'): D[r['address']] = r
    return D
def names():
    S = {}
    for tag, p in (('Ego_r.pdb', ROOT + '/ghidra_out/egor_pdb_names.tsv'), ('FableWin.pdb', ROOT + '/ghidra_out/fablewin_pdb_names.tsv')):
        s = set()
        for l in open(p, encoding='utf-8', errors='replace'):
            q = l.rstrip('\n').split('\t')
            if len(q) >= 2: s.add(q[1])
        S[tag] = s
    return S
def bindings():
    by_t = collections.defaultdict(set); by_s = {}
    for r in csv.reader(open(ROOT + '/rebuild/oracles/reloc_bindings.tsv'), delimiter='\t'):
        if len(r) >= 3 and r[0] != 'symbol': by_t[r[1]].add(r[0]); by_s.setdefault(r[0], (r[1], r[2]))
    return by_t, by_s
def index(p=W + '/idx/full.jsonl'):
    import json
    E = collections.defaultdict(set); ROWS = {}
    for l in open(p):
        r = json.loads(l); ROWS[r['a']] = r
        for s in r.get('u', []): E[s].add(r['a'])
        for s in r.get('d', []): E[s].add(r['a'])
    return E, ROWS
