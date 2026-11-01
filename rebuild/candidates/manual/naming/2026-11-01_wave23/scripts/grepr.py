# grepr.py <rows.txt> <regex> [-c|-l|-L]: grep the landed sources of a row list (fast: one rows() parse)
import sys, os, re, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from rows import rows, ROOT
R = rows(); A = [l.split()[0] for l in open(sys.argv[1]) if l.strip()]
rx = re.compile(sys.argv[2]); mode = sys.argv[3] if len(sys.argv) > 3 else ''
C = collections.Counter()
for a in A:
    p = os.path.join(ROOT, 'rebuild/src/compiled', R[a]['Source']); t = open(p, errors='replace').read()
    m = rx.findall(t)
    if mode == '-c': C.update(set(x if isinstance(x, str) else x[0] for x in m))
    elif mode == '-l':
        if m: print(a)
    elif mode == '-L':
        if not m: print(a)
    else:
        for i, l in enumerate(t.split('\n'), 1):
            if rx.search(l): print('%s:%d: %s' % (a, i, l.rstrip()[:220]))
if mode == '-c':
    for k, v in C.most_common(40): print(v, k)
