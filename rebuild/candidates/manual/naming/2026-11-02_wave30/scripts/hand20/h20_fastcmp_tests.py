# h20_fastcmp_tests.py: family fastcmp -- the three rows whose self-contained tests supply the CCharString comparator body for the link
# (009e5120 / 009e51c0 / 00a6f290): the test's FastCompare becomes operator< with the source (recheck LINK_FAIL otherwise).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_fastcmp.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())
for a in ('009e5120', '009e51c0', '00a6f290'):
    if a in _X: continue
    he.resub(a, 'tst', r'\bFastCompare\b', 'operator<')
