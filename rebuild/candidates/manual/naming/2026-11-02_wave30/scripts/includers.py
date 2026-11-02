# includers.py <header rel> [out rows file]: landed rows whose source or test includes the header directly or through another header
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from rows import rows, ROOT
INC = ROOT + '/rebuild/include'
RX = re.compile(r'#include "([^"]+\.h)"')
target = sys.argv[1]
H = {}
for dp, dn, fn in os.walk(INC):
    for f in fn:
        if f.endswith('.h'):
            p = os.path.join(dp, f); H[os.path.relpath(p, INC)] = set(RX.findall(open(p, encoding='utf-8', errors='replace').read()))
par = {target}; ch = True
while ch:
    ch = False
    for h, d in H.items():
        if h not in par and d & par: par.add(h); ch = True
print('headers:', sorted(par), file=sys.stderr)
R = rows(); out = []
for a, d in R.items():
    for k, base in (('Source', 'rebuild/src/compiled/'), ('TestSource', 'rebuild/tests/')):
        p = os.path.join(ROOT, base + d.get(k, ''))
        if d.get(k) and os.path.exists(p) and set(RX.findall(open(p, encoding='utf-8', errors='replace').read())) & par:
            out.append(a); break
out = sorted(set(out))
if len(sys.argv) > 2: open(sys.argv[2], 'w').write(''.join(a + '\n' for a in out))
print(len(out))
