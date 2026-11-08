#!/usr/bin/env python3
"""Does the source path length / /Fd path change VC7.1 codegen (MSVC 5 symbol-id effect)? Compile one source at
paths of different lengths and hash the function's .text bytes."""
import sys, os, hashlib, shutil
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from pathlib import Path
S=Path(os.environ['S']); A=S/'w22'/'research861'
src=Path(sys.argv[1]); leaf=sys.argv[2]; e=V.env()
for n in (1, 2, 3, 4, 8, 16, 30, 60, 100):
    d=A/'tmp'/'pl'/('p'*n); d.mkdir(parents=True, exist_ok=True)
    sp=d/src.name; shutil.copy(src, sp); obj=d/(src.stem+'.obj'); obj.unlink(missing_ok=True)
    cp=V.cl(["/nologo","/c"]+V.BASE_FLAGS+[f"/Fo{obj}", str(sp)], e)
    if not obj.exists(): print(n, 'FAIL', cp.stdout[-300:]); continue
    try: b=V.obj_text(obj, leaf)[0]
    except Exception as ex: print(n, "objdump fail"); continue
    print(f'dir-name length {n:3d}  path length {len(str(sp)):3d}  bytes {len(b)}  md5 {hashlib.md5(b).hexdigest()[:12]}')
# /Fd variant: name a pdb of different lengths with /Zi? retail had no /Zi for release; try /Fd without /Zi (ignored). Skip.
