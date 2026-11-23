#!/usr/bin/env python3
"""Delete files or directories, but only inside one agent folder (never `rm` a variable path).
   clean.py <agent_folder> <path> [<path> ...]
Refuses any path that does not resolve (symlinks NOT followed for the target itself) strictly inside <agent_folder>."""
import os, shutil, sys
root = os.path.realpath(sys.argv[1]).rstrip('/') + '/'
if root.count('/') < 4: sys.exit('refusing: agent folder too shallow: ' + root)
for p in sys.argv[2:]:
    ap = os.path.abspath(p)
    parent = os.path.realpath(os.path.dirname(ap)).rstrip('/') + '/'
    tgt = parent + os.path.basename(ap)
    if not tgt.startswith(root) or tgt.rstrip('/') + '/' == root:
        print('REFUSED', p); continue
    if os.path.islink(tgt) or os.path.isfile(tgt): os.unlink(tgt); print('removed', tgt)
    elif os.path.isdir(tgt): shutil.rmtree(tgt); print('removed dir', tgt)
    else: print('absent', tgt)
