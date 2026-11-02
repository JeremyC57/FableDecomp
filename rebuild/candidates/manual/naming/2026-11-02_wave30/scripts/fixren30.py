# fixren30.py: post-fixes RENAMES.tsv from mkren30.py (wave 30: nothing dropped unless listed in DROP with its reason).
import os
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
p = os.path.join(W, 'RENAMES.tsv'); T = [l.rstrip('\n').split('\t') for l in open(p)]
DROP = {}
out = [T[0]] + [r for r in T[1:] if r[0] not in DROP]
open(p, 'w').write(''.join('\t'.join(r) + '\n' for r in out)); print(len(out) - 1, 'renames after fix')
