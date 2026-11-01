# funclets.py <rows.txt>: landed rows whose source #includes the source of one of the given rows (funclet / shared-TU dependents)
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rows
R = rows.rows()
want = {os.path.basename(R[l.split()[0]]['Source']): l.split()[0] for l in open(sys.argv[1]) if l.strip() and l.split()[0] in R}
out = set()
for a, r in R.items():
    if a in want.values(): continue
    p = os.path.join(rows.ROOT, 'rebuild/src/compiled', r['Source'])
    try: t = open(p, errors='replace').read(4000)
    except OSError: continue
    for m in re.finditer(r'#include\s+"([^"]+)"', t):
        b = os.path.basename(m.group(1))
        if b in want: out.add((a, want[b]))
for a, p in sorted(out): print(a, p)
