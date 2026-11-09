#!/usr/bin/env python3
"""mc.py <mini.cpp> [extra cl flags]: compile a mini TU under the verifier's base flags (VC7.1 via wine) and print objdump -d of
.text (Intel syntax, relocs). For quick c2 behaviour experiments."""
import sys, os, subprocess
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
src = os.path.abspath(sys.argv[1]); extra = sys.argv[2:]
obj = src[:-4] + '.obj'
e = V.env()
cp = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + extra + [f"/Fo{obj}", src], e)
if not os.path.exists(obj) or cp.returncode:
    print(cp.stdout[-3000:], cp.stderr[-2000:]); sys.exit(1)
out = subprocess.run(['objdump', '-d', '-r', '-M', 'intel', '--no-show-raw-insn', obj], capture_output=True, text=True).stdout
print(out)
