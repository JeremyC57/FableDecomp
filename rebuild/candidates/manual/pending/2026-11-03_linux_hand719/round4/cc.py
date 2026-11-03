#!/usr/bin/env python3
# cc.py <src> [extra flags] -- compile only (no grading) into $S/wine/cc/<name>.obj and list the leaf's call targets
import sys, os, subprocess
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
S = Path(os.environ["S"]); src = Path(sys.argv[1]); extra = sys.argv[2:]
work = S/"wine"/"cc719c"; work.mkdir(parents=True, exist_ok=True)
cpp = work/(src.stem+".cpp"); cpp.write_text(V.vc71(src.read_text()), encoding="utf-8")
obj = work/(src.stem+".obj")
e = V.env()
cp = V.cl(["/nologo","/c"]+V.BASE_FLAGS+extra+[f"/Fo{obj}", str(cpp)], e)
if not obj.exists() or cp.returncode: print(cp.stdout[-3000:], cp.stderr[-2000:]); sys.exit(1)
print(subprocess.run(["objdump","-dr","--no-show-raw-insn",str(obj)],capture_output=True,text=True).stdout)
