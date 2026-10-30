#!/usr/bin/env python3
"""cerr.py <file>: compile one worktree source under the base flags (no variant search) and print the first errors."""
import sys, os, subprocess
ROOT = os.environ["FABLE_ROOT"]; sys.path.insert(0, ROOT + "/tools/decomp_pipeline")
import verify_and_land as V
S = os.environ["S"]; w = S + "/mig9/work/cerr"; os.makedirs(w, exist_ok=True)
src = open(sys.argv[1], encoding="utf-8").read()
p = w + "/t.cpp"; open(p, "w").write(V.vc71(src))
cp = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + ["/Fo" + w + "/t.obj", p], V.env())
errs = [l for l in (cp.stdout or "").splitlines() if "error" in l]
print("\n".join(l.split("t.cpp")[-1][:300] for l in errs[:int(sys.argv[2]) if len(sys.argv) > 2 else 8]) or "OK")
