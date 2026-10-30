# ovrows.py <overlay> : landed rows whose source or test in the overlay differs from the repo
import sys, os, filecmp
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from rows import rows, ROOT
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
R = rows(); ov = os.path.join(W, sys.argv[1]); out = []
for a, d in R.items():
    for rel in ('rebuild/src/compiled/' + d['Source'], 'rebuild/tests/' + d['TestSource']):
        p = os.path.join(ov, rel)
        if os.path.exists(p) and not filecmp.cmp(p, os.path.join(ROOT, rel), shallow=False): out.append(a); break
print('\n'.join(sorted(out)))
