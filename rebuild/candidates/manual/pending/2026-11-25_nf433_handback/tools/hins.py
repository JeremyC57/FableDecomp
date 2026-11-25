#!/usr/bin/env python3
"""hins.py <header> <anchor-line-substring> <blockfile> [after|before]: insert the block once, after (default) the FIRST line containing the anchor.
Refuses if the block's first line is already present (idempotent)."""
import sys
h, anc, bf = sys.argv[1:4]; mode = sys.argv[4] if len(sys.argv) > 4 else 'after'
L = open(h).read().split('\n'); B = open(bf).read().rstrip('\n').split('\n')
if '\n'.join(B) in '\n'.join(L): print('already', h); sys.exit(0)
i = next(k for k, l in enumerate(L) if anc in l)
L[i + (1 if mode == 'after' else 0):i + (1 if mode == 'after' else 0)] = B
open(h, 'w').write('\n'.join(L)); print('inserted', h, 'at', i + 1)
