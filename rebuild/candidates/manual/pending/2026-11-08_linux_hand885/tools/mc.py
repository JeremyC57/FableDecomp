#!/usr/bin/env python3
"""mc.py <src.cpp> <workdir>: compile a full-row variant (overlay via FABLE_HDR_OVERLAY, `// Flags:` honoured) into
<workdir>/m.obj without touching the q.py grade dir (session 4: lets one compile run beside a grade)."""
import sys, os, re
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from pathlib import Path
src = Path(sys.argv[1]).read_text(encoding='utf-8'); work = Path(sys.argv[2]); work.mkdir(parents=True, exist_ok=True)
m = re.search(r'^// Flags: (.*)$', src, re.M)
extra = m.group(1).split() if m else []
(work/'m.cpp').write_text(V.vc71(src), encoding='utf-8')
if (work/'m.obj').exists(): (work/'m.obj').unlink()
cp = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + extra + [f"/Fo{work/'m.obj'}", str(work/'m.cpp')], V.env())
if not (work/'m.obj').exists() or cp.returncode:
    print('SRC_FAIL'); print(cp.stdout[-3000:], cp.stderr[-2000:]); sys.exit(1)
print('ok', work/'m.obj')
