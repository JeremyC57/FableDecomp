# renscan.py: per final row whose overlay source carries a Leaf, the Leaf's qualified name against the row's Module / Source
import os, sys, re, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import und
from rows import rows
R = rows(); W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
A = [l.strip() for l in open(os.path.join(W, 'runs/final.rows.txt')) if l.strip()]
leaf = {}
for a in A:
    p = os.path.join(W, 'ov_final/rebuild/src/compiled', R[a]['Source'])
    if not os.path.exists(p): continue
    t = open(p, errors='replace').read()
    m = re.search(r'^// Leaf: (\S+)', t, re.M)
    if m: leaf[a] = m.group(1)
D = und.get(list(leaf.values()))
for a in A:
    if a not in leaf: print(a, 'NOLEAF', R[a]['Module'], R[a]['Source']); continue
    q = und.qname(D[leaf[a]])
    print(a, q, '|', R[a]['Module'], '|', R[a]['Source'].split('/')[-1])
