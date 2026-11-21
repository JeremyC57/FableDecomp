#!/usr/bin/env python3
"""odl.py <src.cpp> <func-substring> [flags]: VC7.1 /Od /FAsc listing -> bytes per source line for one function."""
import sys, re
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from pathlib import Path
src = Path(sys.argv[1]).resolve(); fn = sys.argv[2]; extra = sys.argv[3:]
e = V.env(); asm = src.parent / (src.stem + "_od.cod")
cp = V.cl(["/nologo", "/c", "/Od", "/W3"] + extra + ["/FAsc", f"/Fa{asm}", f"/Fo{src.parent/(src.stem+'_od.obj')}", str(src)], e)
if cp.returncode: print(cp.stdout[-2000:]); sys.exit(1)
inf = False; cur = None; sizes = {}; order = []
for l in asm.read_text(errors="replace").splitlines():
    if re.match(r"^\S+\s+PROC", l) and fn in l: inf = True; continue
    if inf and re.match(r"^\S+\s+ENDP", l): break
    if not inf: continue
    m = re.match(r"^; (\d+)\s*:", l)
    if m: cur = int(m.group(1)); order.append(cur) if cur not in sizes else None; sizes.setdefault(cur, 0); continue
    m = re.match(r"^\s+[0-9a-f]{5}\s+((?:[0-9a-f]{2}\s?)+)", l)
    if m and cur is not None: sizes[cur] += len(m.group(1).split())
for k in order: print("  line %-5d %4d B" % (k, sizes[k]))
