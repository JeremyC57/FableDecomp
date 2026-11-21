import sys, re
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from pathlib import Path
w = Path(sys.argv[1]).resolve(); e = V.env(); extra=sys.argv[2:]
cp = V.cl(["/nologo","/c"]+V.BASE_FLAGS+extra+['/Ycpch.h', f'/Fp{w/"p.pch"}', f'/Fo{w/"pch.obj"}', str(w/'pch.cpp')], e)
print("yc", cp.returncode, cp.stdout[-500:])
cp = V.cl(["/nologo","/c"]+V.BASE_FLAGS+extra+['/Yupch.h', f'/Fp{w/"p.pch"}', "/FAs", f"/Fa{w/'m5.asm'}", f'/Fo{w/"m5.obj"}', str(w/'m5.cpp')], e)
print("yu", cp.returncode, cp.stdout[-500:])
for l in (w/'m5.asm').read_text(errors='replace').splitlines():
    if '\t' in l and not l.startswith(';'): print(l)
