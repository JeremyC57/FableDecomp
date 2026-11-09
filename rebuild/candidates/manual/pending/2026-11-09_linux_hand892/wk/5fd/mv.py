#!/usr/bin/env python3
"""mv.py <base.cpp> <variants.py> <sym>: apply each (tag, [(old,new)]) to base, micro-compile, print skeleton."""
import sys, subprocess, os
A = os.environ["S"] + "/w22/agent892"
base, vf, sym = sys.argv[1:4]
src = open(base).read(); ns = {"src": src}; exec(open(vf).read(), ns)
for tag, reps in ns["V"]:
    s = src
    for o, n in reps:
        if o not in s: print(tag, "MISSING", repr(o[:60])); s = None; break
        s = s.replace(o, n)
    if s is None: continue
    p = f"{A}/wk/5fd/v_{tag}.cpp"; open(p, "w").write(s)
    r = subprocess.run(["python3", f"{A}/wk/5fd/skel.py", p, sym], capture_output=True, text=True)
    print(tag, "::", r.stdout.strip()[:900], r.stderr.strip()[-300:], flush=True)
