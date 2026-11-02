# fixren28.py: post-fixes RENAMES.tsv from mkren28.py (wave 28: nothing dropped unless listed in DROP with its reason).
import os
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
p = os.path.join(W, 'RENAMES.tsv'); T = [l.rstrip('\n').split('\t') for l in open(p)]
DROP = {'00bdfa10': 'the row\'s own Leaf is its member stand-in ScalarDeletingDestructor (not the compiler ??_G): only the callee was respelled this wave; own-row re-author deferred'}
out = [T[0]] + [r for r in T[1:] if r[0] not in DROP]
open(p, 'w').write(''.join('\t'.join(r) + '\n' for r in out)); print(len(out) - 1, 'renames after fix')
