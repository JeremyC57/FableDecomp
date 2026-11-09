#!/usr/bin/env python3
"""var.py <addr> <base.cpp> <tag> <old> <new> [<old> <new> ...]  -> probe/<addr>_<tag>.cpp, then grade it (gr.sh)."""
import sys, subprocess, os
A = "/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent892"
addr, base, tag = sys.argv[1:4]; reps = sys.argv[4:]
s = open(base).read()
for i in range(0, len(reps), 2):
    o, n = reps[i], reps[i+1]
    if s.count(o) < 1: sys.exit(f"MISSING: {o!r}")
    s = s.replace(o, n)
p = f"{A}/probe/{addr}_{tag}.cpp"; open(p, "w").write(s)
r = subprocess.run(["bash", f"{A}/tools/gr.sh", addr, p, tag], capture_output=True, text=True)
print((r.stdout + r.stderr).strip().split("\t", 1)[-1][:200])
