# autoscan.py: queue rows whose PDB-spelled final differs from a bound, emitted spelling only in the member access/const code
import sys, os, re, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs
BT, BS = inputs.bindings(); E, R = inputs.index(); N = inputs.names()
PDB = N['Ego_r.pdb'] | N['FableWin.pdb']
MEM = re.compile(r'^\?(\w+)@(\w+)@@([AIQ])([AB])E(.*)$')
out = []
seen = set()
for q in inputs.queue():
    fin = q[2].split()[0] if len(q) > 2 and q[2].split() else ''
    m = MEM.match(fin)
    if not m or fin not in PDB: continue
    t = inputs.na(q[1].split()[0])
    if (t, fin) in seen: continue
    seen.add((t, fin))
    for s in BT.get(t, ()):
        if s == fin: continue
        m2 = MEM.match(s)
        if not m2 or m2.group(1) != m.group(1) or m2.group(2) != m.group(2) or m2.group(5) != m.group(5): continue
        rows = sorted(E.get(s, ()))
        if rows: out.append((t, fin, s, rows))
for t, fin, s, rows in out: print(t, fin, s, len(rows), ' '.join(rows[:20]))
print('pairs', len(out), 'rows', sum(len(r) for *_, r in out), file=sys.stderr)
