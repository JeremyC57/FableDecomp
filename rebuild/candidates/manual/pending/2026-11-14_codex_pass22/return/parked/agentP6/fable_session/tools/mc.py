#!/usr/bin/env python3
"""mc.py <src.cpp> [flags...] : compile a micro TU with the verifier's base flags, print each function's disassembly (objdump)."""
import sys, os, subprocess
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
src = Path(sys.argv[1]).resolve(); extra = sys.argv[2:]
e = V.env(); obj = src.with_suffix(".obj")
cp = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + extra + [f"/Fo{obj}", str(src)], e)
if cp.returncode: print(cp.stdout[-3000:], cp.stderr[-1000:]); sys.exit(1)
out = subprocess.run(["objdump", "-d", "-r", "-M", "intel", "--no-show-raw-insn", str(obj)], capture_output=True, text=True).stdout
print(out)
