# ana10.py <targets.tsv: addr \t final> : per target, the bound spellings and the indexed rows that still emit each
import sys, os, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs
BT, BS = inputs.bindings(); NM = inputs.names(); E, ROWS = inputs.index(os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'idx/full.jsonl'))
for l in open(sys.argv[1]):
    q = l.rstrip('\n').split('\t')
    if len(q) < 2 or l.startswith('#'): continue
    t, fin = q[0], q[1]
    print('== %s %s [%s%s] emitters %d' % (t, fin, 'E' if fin in NM['Ego_r.pdb'] else '-', 'F' if fin in NM['FableWin.pdb'] else '-', len(E.get(fin, ()))))
    for s in sorted(BT.get(t, ())):
        if s == fin: continue
        em = sorted(E.get(s, ()))
        print('   %-3d %s %s' % (len(em), s[:160], ' '.join(em)))
