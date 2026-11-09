#!/usr/bin/env python3
"""mcfa.py <src.cpp> <workdir>: like tools/mc.py (plain compile, `// Flags:` honoured) plus a /FAs listing:
<workdir>/m.obj and <workdir>/m.asm (source lines + symbolic frame names)."""
import sys, os, re
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe")
import verify_and_land as V
import c2dbg
from pathlib import Path
src = Path(sys.argv[1]).read_text(encoding='utf-8'); work = Path(sys.argv[2]).resolve(); work.mkdir(parents=True, exist_ok=True)
m = re.search(r'^// Flags: (.*)$', src, re.M)
extra = m.group(1).split() if m else []
(work/'m.cpp').write_text(V.vc71(src), encoding='utf-8')
for f in ('m.obj', 'm.asm'):
    if (work/f).exists(): (work/f).unlink()
cp = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + extra + ['/FAs', '/Fa' + c2dbg.winpath(str(work/'m.asm')), f"/Fo{work/'m.obj'}", str(work/'m.cpp')], V.env())
if not (work/'m.obj').exists() or cp.returncode:
    print('SRC_FAIL'); print(cp.stdout[-3000:], cp.stderr[-2000:]); sys.exit(1)
print('ok', work/'m.obj')
