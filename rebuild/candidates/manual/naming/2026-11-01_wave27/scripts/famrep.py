# famrep.py <target> <final> [rows_out]: rows (index) that name a non-final bound spelling of the target, grouped by spelling;
# rows that name the final spelling are counted. rows_out: write the emitting rows (one per line).
import sys, os, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs
from rows import rows
R = rows()
t, fin = inputs.na(sys.argv[1]), sys.argv[2]
BT, BS = inputs.bindings(); E, IX = inputs.index()
asm = lambda a: ('ASM' if any(k in open('/home/user/FableDecomp/rebuild/src/compiled/' + R[a]['Source'], errors='replace').read() for k in ('__asm', '_emit', 'naked')) else '')
allr = set()
print('final', fin, 'named by', len(E.get(fin, ())), 'rows')
for s in sorted(BT.get(t, ()), key=lambda s: -len(E.get(s, ()))):
    if s == fin: continue
    rs = sorted(E.get(s, ()))
    allr |= set(rs)
    print('%3d %s  %s' % (len(rs), s[:150], ' '.join(a + asm(a) for a in rs[:40])))
print('rows', len(allr))
if len(sys.argv) > 3: open(sys.argv[3], 'w').write(''.join(a + '\n' for a in sorted(allr)))
