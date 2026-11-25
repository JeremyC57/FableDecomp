#!/usr/bin/env python3
"""grade.py <tag> <addr>=<src>[@<flags-from-addr>] ...: read-only VC7.1 grade of a probe source against the catalog oracle of <addr>
(leaf from the source's // Leaf line, else the catalog name); objects -> r2/obj/<tag>/. Probes only: no row is touched."""
import sys, shutil, subprocess
from pathlib import Path
A = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/"
sys.path.insert(0, A + "tools"); import nf
sys.path.insert(0, str(nf.R / "tools/decomp_pipeline")); import verify_and_land as V
tag = sys.argv[1]; od = Path(A + "r3/obj/" + tag); od.mkdir(parents=True, exist_ok=True)
log = open(A + "r3/work/grade_%s.log" % tag, "a")
for arg in sys.argv[2:]:
    a, rest = arg.split("=", 1)
    rest, _, xf = rest.partition("!")
    src, _, fa = rest.partition("@")
    text = open(src, encoding="utf-8").read()
    fl = nf.flags_of(fa or a, nf.src_of(fa or a).read_text(encoding="utf-8") if nf.src_of(fa or a) else text)
    if xf: fl = [x for x in xf.split(",") if x]
    retail = bytes.fromhex(nf.catalog()[a]["bytes"])
    while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]
    wd = nf.W / "g" / (a + "_gr" + tag); wd.mkdir(parents=True, exist_ok=True)
    leaf = nf.leaf_of(text, a)
    try: st, built, sec = V.parity_of(V.vc71(text), a, leaf, retail, wd, V.env(), extra=tuple(fl))
    except Exception as ex: st = "ERR " + repr(ex)[:200]
    obj = wd / (a + ".obj")
    if obj.exists(): shutil.copy(obj, od / (a + ".obj"))
    print(a, st, "leaf=" + leaf, "flags=" + " ".join(fl), file=log); log.flush()
    subprocess.run(["python3", str(nf.R / "tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py"), str(nf.A), str(wd)], capture_output=True)
print("GRADE_DONE", file=log)
