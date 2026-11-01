# emit.py <target...>: every bound spelling of each retail target, the rows whose object names it (index), PDB flags
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs
BT, BS = inputs.bindings(); E, R = inputs.index(); N = inputs.names()
for a in sys.argv[1:]:
    t = inputs.na(a)
    print('##', t)
    for s in sorted(BT.get(t, ()), key=lambda s: -len(E.get(s, ()))):
        pdb = ('E' if s in N['Ego_r.pdb'] else '-') + ('F' if s in N['FableWin.pdb'] else '-')
        rs = sorted(E.get(s, ()))
        print('  %s %3d %s  %s' % (pdb, len(rs), s, ' '.join(rs[:30])))
