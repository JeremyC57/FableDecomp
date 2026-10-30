# symdiff.py <overlay> <addr...>: compile the repo and overlay sources of each row, print defined/undefined symbol changes
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib
from rows import ROOT
ov = sys.argv[1]
for a in sys.argv[2:]:
    rel = 'rebuild/src/compiled/' + famlib.R[a]['Source']
    S0, e0 = famlib.objsyms(a, os.path.join(ROOT, rel), 'sd0')
    S1, e1 = famlib.objsyms(a, os.path.join(famlib.W, ov, rel), 'sd1')
    if S1 is None: print(a, 'COMPILE FAIL', e1); continue
    print(a, '+', sorted(S1 - S0)); print(a, '-', sorted(S0 - S1))
