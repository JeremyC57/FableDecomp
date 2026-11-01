# funclets13.py <rows.txt> [out.txt]: the landed unwind-funclet stubs whose source includes an edited parent row's source
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from rows import rows, ROOT
R = rows(); src2a = {d['Source']: a for a, d in R.items() if 'Source' in d}
P = {}
for a, d in R.items():
    s = d.get('Source', '')
    if '/global_Unwind' not in s and 'Unwind' not in s: continue
    try: t = open(os.path.join(ROOT, 'rebuild/src/compiled', s), errors='replace').read()
    except OSError: continue
    for m in re.finditer(r'#include "\.\./src/compiled/([^"]+)"', t):
        pa = src2a.get(m.group(1)) or (re.search(r'_([0-9a-f]{8})\.cpp$', m.group(1)) or [None, None])[1]
        if pa in R: P.setdefault(pa, set()).add(a)
ed = set(l.strip() for l in open(sys.argv[1]) if l.strip())
out = sorted(f for a in ed for f in P.get(a, ()))
print(len(out), 'funclet dependents')
if len(sys.argv) > 2: open(sys.argv[2], 'w').write(''.join(a + '\n' for a in out))
