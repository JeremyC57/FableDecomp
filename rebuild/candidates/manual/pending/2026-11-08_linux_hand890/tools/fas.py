#!/usr/bin/env python3
"""fas.py <src.cpp> <outdir> [extra cl flags...]: compile with the lane's base flags + /FAs (header overlay from
FABLE_HDR_OVERLAY) and write <outdir>/<stem>.asm and .obj; prints the frame-symbol table (`_name$N = off`) of every PROC."""
import sys, os, re
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
src = Path(sys.argv[1]); out = Path(sys.argv[2]); out.mkdir(parents=True, exist_ok=True)
extra = sys.argv[3:]
fl = re.search(r'^// Flags: (.+)$', src.read_text(), re.M)
if fl and fl.group(1).strip() != 'none': extra = fl.group(1).split() + extra
w = out / (src.stem + '.cpp'); w.write_text(V.vc71(src.read_text()))
e = V.env()
cp = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + extra + ["/FAs", f"/Fa{out/(src.stem+'.asm')}", f"/Fo{out/(src.stem+'.obj')}", str(w)], e)
if cp.returncode:
    print(cp.stdout[-3000:], cp.stderr[-2000:]); sys.exit(1)
asm = (out / (src.stem + '.asm')).read_text(errors='replace')
want = os.environ.get('FUNC')
for m in re.finditer(r'((?:^_?\$?[\w$?@]+\$\d*\s*=\s*-?\d+\s*;.*\n)+)(^\S+\s+PROC.*$)', asm, re.M):
    if want and want not in m.group(2): continue
    print(m.group(2)); print(m.group(1))
