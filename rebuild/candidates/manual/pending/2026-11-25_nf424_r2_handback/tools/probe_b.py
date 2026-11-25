#!/usr/bin/env python3
"""probe_b.py: R-NF424-1 (b) settlement. For each held G1 row, the probe TU is the row's own text (same prelude + shared header) plus
the class's Clone defined as retail's body `return new C(*this);` (the header declares it); graded on the Clone leaf against the landed
Clone row's retail bytes. RELOCATION_MATCH = the shared header's implicit copy constructor reproduces retail's inline copy in Clone.
Read-only grading (no row touched). Writes r2/work/probe_b.log; objects r2/probe/<addr>.obj."""
import sys, os, re, shutil, subprocess
from pathlib import Path
A = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/"
sys.path.insert(0, A + "tools"); import nf
sys.path.insert(0, str(nf.R / "tools/decomp_pipeline")); import verify_and_land as V
log = open(A + "r2/work/probe_b.log", "a")
for arg in sys.argv[1:]:
    g, c, cls = arg.split(":")
    src = open(os.environ.get("PROBE_SRC", A + "rows") + "/%s.cpp" % g).read()
    src = re.sub(r"^// Leaf:.*$", "// Leaf: ?Clone@%s@@UBEPAVCCreatureActionBase@@XZ" % cls, src, flags=re.M)
    src += "\nCCreatureActionBase* %s::Clone() const\n{\n    return new %s(*this);\n}\n" % (cls, cls)
    open(A + "r2/probe/%s.cpp" % g, "w").write(src)
    retail = bytes.fromhex(nf.catalog()[c]["bytes"])
    while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]
    wd = nf.W / "g" / (c + "_probeb"); wd.mkdir(parents=True, exist_ok=True)
    fl = nf.flags_of(g, src)
    try:
        st, built, sec = V.parity_of(V.vc71(src), c, "?Clone@%s@@UBEPAVCCreatureActionBase@@XZ" % cls, retail, wd, V.env(), extra=tuple(fl))
    except Exception as ex:
        st = "ERR " + repr(ex)[:200]
    obj = wd / (c + ".obj")
    if obj.exists(): shutil.copy(obj, A + "r2/probe/%s.obj" % g)
    print(g, c, cls, st, "flags=" + " ".join(fl), file=log); log.flush()
    subprocess.run(["python3", str(nf.R / "tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py"), str(nf.A), str(wd)], capture_output=True)
print("PROBE_DONE", file=log)
