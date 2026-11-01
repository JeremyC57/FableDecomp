# fixren26.py: post-fixes RENAMES.tsv from mkren26.py:
#  - 0x007AD180 / 0x007DC750 keep their file and module: the function token is unchanged (erase / insert) and only the module would
#    expand to the full STLport template spelling (the catalog carries both short and long std:: module forms; 6 rows use
#    `std::vector<long>`) -- not a wrong name, so no rename.
import os
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
p = os.path.join(W, 'RENAMES.tsv'); T = [l.rstrip('\n').split('\t') for l in open(p)]
DROP = {'007ad180', '007dc750'}
out = [T[0]] + [r for r in T[1:] if r[0] not in DROP]
open(p, 'w').write(''.join('\t'.join(r) + '\n' for r in out)); print(len(out) - 1, 'renames after fix')
