#!/usr/bin/env python3
"""cc.py <src.cpp> [timeout_s] : compile only (no grade), print errors; kills its own cl on timeout."""
import sys, os, subprocess
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
W = Path(os.environ["W"]); src = Path(sys.argv[1]); to = int(sys.argv[2]) if len(sys.argv) > 2 else 240
work = W/"tmp"/"cc"; work.mkdir(parents=True, exist_ok=True)
sp = work/"cc.cpp"; sp.write_text(V.vc71(src.read_text()))
e = V.env()
args = ([V.WINE] if V.WINE else []) + [str(V.CL_EXE)] + V.tool_args(["/nologo", "/c"] + V.BASE_FLAGS + [f"/Fo{work/'cc.obj'}", str(sp)])
p = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, env=e)
try:
    out = p.communicate(timeout=to)[0]; print("rc", p.returncode); print(out[-3000:])
except subprocess.TimeoutExpired:
    p.kill(); subprocess.run(["pkill", "-f", "hand2c.tmp.cc.cc.cpp"]); print("TIMEOUT")
