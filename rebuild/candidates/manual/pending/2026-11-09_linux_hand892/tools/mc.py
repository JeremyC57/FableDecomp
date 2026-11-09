#!/usr/bin/env python3
"""mc.py <src.cpp> [sym-substring] [--flags ...]: compile a micro TU with the lane's VC7.1 (verifier base flags) and print
the disassembly of the functions whose symbol contains the substring."""
import sys, os, subprocess, re
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
S = Path(os.environ["S"]); A = S/"w22"/"agent892"
args = sys.argv[1:]; extra = []
if "--qfe" in args: args.remove("--qfe"); V.use_qfe()
if "--flags" in args: i = args.index("--flags"); extra = args[i+1:]; args = args[:i]
src = Path(args[0]); pat = args[1] if len(args) > 1 else ""
work = A/"wk"/"mc"; work.mkdir(parents=True, exist_ok=True)
sp = work/"m.cpp"; sp.write_text(V.vc71(src.read_text())); obj = work/"m.obj"; obj.unlink(missing_ok=True)
cp = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + extra + [f"/Fo{obj}", str(sp)], V.env())
if not obj.exists(): print(cp.stdout[-3000:]); sys.exit(1)
out = subprocess.run(["objdump", "-d", "-M", "intel", "--no-show-raw-insn", str(obj)], capture_output=True, text=True).stdout
show = False
for l in out.splitlines():
    m = re.match(r"^[0-9a-f]+ <(.+)>:$", l)
    if m: show = pat in m.group(1); 
    if show: print(l)
