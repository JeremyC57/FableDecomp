# stubs.py <rows> [out]: landed rows whose source includes "../src/compiled/<parent>" of a listed row (unwind-funclet stubs)
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from rows import rows, ROOT
R = rows(); A = set(l.split()[0] for l in open(sys.argv[1]) if l.strip())
srcs = {R[a]['Source']: a for a in A}
out = []
for a, d in R.items():
    if a in A or not d.get('Source'): continue
    p = os.path.join(ROOT, 'rebuild/src/compiled', d['Source'])
    if not os.path.exists(p): continue
    t = open(p, encoding='utf-8', errors='replace').read()
    for m in re.finditer(r'#include "\.\./src/compiled/([^"]+)"', t):
        if m.group(1) in srcs: out.append(a)
out = sorted(set(out)); print(len(out), ' '.join(out[:20]))
if len(sys.argv) > 2: open(sys.argv[2], 'w').write(''.join(a + '\n' for a in out))
