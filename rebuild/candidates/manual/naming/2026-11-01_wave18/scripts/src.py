# src.py <row> [regex]: path of a landed row's source; with a regex, matching lines (+line numbers)
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from rows import rows, ROOT
R = rows()
a = sys.argv[1].lower().replace('0x', '')
p = os.path.join(ROOT, 'rebuild/src/compiled', R[a]['Source'])
if len(sys.argv) < 3: print(p); sys.exit()
for i, l in enumerate(open(p, errors='replace'), 1):
    if re.search(sys.argv[2], l): print('%s:%d: %s' % (a, i, l.rstrip()[:230]))
