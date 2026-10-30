#!/usr/bin/env python3
"""mbexp.py <file>...: MovieBase experiment (no worktree writes): compile the row and a variant with every
`#pragma inline_depth` line removed, the declared `~CScriptGameResourceObjectMovieBase();` dropped (implicit dtor) and
`__forceinline ~CCountedPointer` made plain inline; print SAME / DIFF of the two leaf byte strings (vtry.py)."""
import sys, os, re, subprocess
S = os.environ["S"]; M = S + "/mig9/work/mbx"
for f in sys.argv[1:]:
    a = re.search(r"_([0-9a-f]{8})\.cpp$", f).group(1)
    t = open(f).read()
    v = "\n".join(l for l in t.split("\n") if not l.strip().startswith("#pragma inline_depth"))
    v = re.sub(r"^\s*~CScriptGameResourceObjectMovieBase\s*\(\s*\)\s*;[^\n]*\n", "", v, flags=re.M)
    v = v.replace("__forceinline ~CCountedPointer", "~CCountedPointer")
    open(M + "/v_%s.cpp" % a, "w").write(v)
    outs = []
    for src in (f, M + "/v_%s.cpp" % a):
        leaf = re.search(r"_([A-Za-z0-9]+)_[0-9a-f]{8}\.cpp$", f).group(1)
        nb = S + "/mig9/work/vtry/%s/new.bin" % a
        if os.path.exists(nb): os.unlink(nb)
        r = subprocess.run([sys.executable, S + "/mig9/scripts/vtry.py", a, src, leaf], capture_output=True, text=True, timeout=600)
        outs.append(((r.stdout + r.stderr[-200:]).strip(), open(nb, "rb").read() if os.path.exists(nb) and "SRC_FAIL" not in r.stdout else b""))
    print(a, "SAME" if outs[0][1] == outs[1][1] and outs[1][1] else "DIFF", outs[0][0][:40], "|", outs[1][0][:60], sep="\t", flush=True)
