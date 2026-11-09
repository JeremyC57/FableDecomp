#!/usr/bin/env python3
"""batch.py <addr> <base.cpp> <variants.py>  -- variants.py defines V = [(tag, [(old, new), ...]), ...]; grades each in turn."""
import sys, subprocess
A = "/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent892"
addr, base, vf = sys.argv[1:4]
ns = {}; exec(open(vf).read(), ns)
for tag, reps in ns["V"]:
    args = ["python3", f"{A}/tools/var.py", addr, base, tag]
    for o, n in reps: args += [o, n]
    r = subprocess.run(args, capture_output=True, text=True)
    out = (r.stdout + r.stderr).strip().split("\n")[-1]
    f = out.split("\t")
    print(tag, f[2] if len(f) > 3 else out[:120], f[3] if len(f) > 3 else "", flush=True)
