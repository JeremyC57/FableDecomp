#!/usr/bin/env python3
"""odall.py <src.cpp> <substr>... [-- flags]: one VC7.1 /Od /FAsc compile; bytes per source line for every PROC matching a substring."""
import sys, re
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from pathlib import Path
a = sys.argv[1:]; extra = []
if "--" in a: i = a.index("--"); extra = a[i+1:]; a = a[:i]
src = Path(a[0]).resolve(); subs = a[1:]
e = V.env(); asm = src.parent / (src.stem + "_od.cod")
inc = []
cp = V.cl(["/nologo", "/c", "/Od", "/W3"] + V.BASE_FLAGS[:0] + extra + ["/FAsc", f"/Fa{asm}", f"/Fo{src.parent/(src.stem+'_od.obj')}", str(src)], e)
if cp.returncode: print(cp.stdout[-3000:]); sys.exit(1)
cur_fn = None; cur = None
for l in asm.read_text(errors="replace").splitlines():
    m = re.match(r"^(\S+)\s+PROC", l)
    if m:
        cur_fn = m.group(1) if any(s in m.group(1) for s in subs) else None
        sizes = {}; order = []; cur = None; continue
    if cur_fn and re.match(r"^\S+\s+ENDP", l):
        print(cur_fn); print("   " + " ".join("%d:%d" % (k, sizes[k]) for k in order)); cur_fn = None; continue
    if not cur_fn: continue
    m = re.match(r"^; (\d+)\s*:", l)
    if m:
        cur = int(m.group(1))
        if cur not in sizes: order.append(cur); sizes[cur] = 0
        continue
    m = re.match(r"^\s+[0-9a-f]{5}\s+((?:[0-9a-f]{2}\s?)+)", l)
    if m and cur is not None: sizes[cur] += len(m.group(1).split())
