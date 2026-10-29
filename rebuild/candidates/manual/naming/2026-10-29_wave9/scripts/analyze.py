# per candidate address: final proposal(s), bound spellings, rows emitting each bound wrong spelling (symbol index)
import sys, os, json, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs, und
Q = inputs.queue(); D8 = inputs.w8(); NM = inputs.names(); BT, BS = inputs.bindings()
E = collections.defaultdict(set)
for f in sys.argv[1:]:
    e, _ = inputs.index(f)
    for k, v in e.items(): E[k] |= v
out = {}
cands = {}
for a, ps in Q.items():
    c = collections.Counter(p[1].replace('_W', 'G') for p in ps); cands[a] = ('queue', c.most_common(1)[0][0])
for a, r in D8.items():
    if r['w8'].startswith('open:') and a not in cands: cands[a] = ('w8:' + r['confidence'], r['final_symbol'])
for a, (src, s) in sorted(cands.items()):
    wrong = sorted(BT.get(a, set()) - {s})
    pdb = '+'.join(t for t in ('Ego_r.pdb', 'FableWin.pdb') if s in NM[t] or s.replace('G', '_W') in NM[t]) or '-'
    em = {w: sorted(E.get(w, ())) for w in wrong}
    out[a] = {'src': src, 'final': s, 'pdb': pdb, 'final_rows': sorted(E.get(s, ())), 'final_bound': BS.get(s), 'wrong': em}
json.dump(out, open(os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'analysis.json'), 'w'), indent=0)
tot = collections.Counter()
for a, o in out.items():
    n = len(set(x for v in o['wrong'].values() for x in v)); tot[o['src']] += n
print(len(out), tot)
