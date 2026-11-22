#!/usr/bin/env python3
"""mc.py <src.cpp> [extra flags...]: compile a mini with VC7.1 3077 base flags (/O2 /Oy /W3) and print the /FAsc listing code lines."""
import sys, os, re
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
src = Path(sys.argv[1]).resolve(); extra = sys.argv[2:]
e = V.env()
out = src.with_suffix(".asm")
cp = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + extra + [f"/Fo{src.with_suffix('.obj')}", f"/Fa{out}", str(src)], e)
if not out.exists(): print(cp.stdout[-3000:], cp.stderr[-2000:]); sys.exit(1)
for l in out.read_text(errors="replace").splitlines():
    if re.match(r"^\s*(;|_TEXT|PUBLIC|EXTRN|INCLUDELIB|TITLE|\.|$|COMM|CONST|_DATA|FLAT|xdata|\$SG)", l): continue
    print(l.rstrip())
