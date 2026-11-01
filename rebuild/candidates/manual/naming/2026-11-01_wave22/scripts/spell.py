# spell.py <target> <final> [spelling-substring]: every non-final bound spelling of target with ALL rows naming it (tsv: spelling \t rows)
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs
t, fin = inputs.na(sys.argv[1]), sys.argv[2]
BT, BS = inputs.bindings(); E, IX = inputs.index()
for s in sorted(BT.get(t, ()), key=lambda s: -len(E.get(s, ()))):
    if s == fin: continue
    if len(sys.argv) > 3 and sys.argv[3] not in s: continue
    print('%s\t%s' % (s, ' '.join(sorted(E.get(s, ())))))
