#!/usr/bin/env python3
"""mini.py <src.cpp> [flags...]: compile a mini TU with VC7.1 /O2 /Oy /W3 /FAs and print the asm listing (code lines only)."""
import sys, os, re
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from pathlib import Path
src = Path(sys.argv[1]).resolve(); extra = sys.argv[2:]
e = V.env()
work = src.parent
asm = work / (src.stem + ".asm")
cp = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + extra + ["/FAs", f"/Fa{asm}", f"/Fo{work/(src.stem+'.obj')}", str(src)], e)
if cp.returncode:
    print(cp.stdout[-2000:], cp.stderr[-1000:]); sys.exit(1)
for l in asm.read_text(errors="replace").splitlines():
    if l.startswith((";", "\t;")) or not l.strip(): continue
    if re.match(r"\s*(PUBLIC|EXTRN|INCLUDELIB|TITLE|\.|_TEXT|_DATA|CONST|COMDAT|ALIGN|ASSUME|FLAT|END\b)", l): continue
    print(l.rstrip())
