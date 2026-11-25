#!/usr/bin/env python3
"""cc2.py <tag> <addr>...: VC7.1 compile + grade of r2/rows/<addr>.cpp (thin row) with the OWNER's compiler flags (catalog CompilerFlags /
// Flags of the owner row, since the object is the owner's TU), graded on the row's leaf against the catalog oracle.
Object -> r2/obj/<tag>/<addr>.obj; status -> r2/work/cc2_<tag>.log. Locks: only rows held by NF424."""
import sys, os, shutil, subprocess
from pathlib import Path
A = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/"
sys.path.insert(0, A + "tools"); import nf
sys.path.insert(0, str(nf.R / "tools/decomp_pipeline")); import verify_and_land as V
tag = sys.argv[1]
gt = {}
for l in open(A + "r2/work/gen_thin.tsv"):
    q = l.rstrip("\n").split("\t"); gt[q[0]] = q
log = open(A + "r2/work/cc2_%s.log" % tag, "a")
od = Path(A + "r2/obj/" + tag); od.mkdir(parents=True, exist_ok=True)
for a in sys.argv[2:]:
    if nf.lock_owner(a) != "NF424": print("NOLOCK", a, file=log); log.flush(); continue
    try:
        owner, osrc = gt[a][2], gt[a][3]
        src = open(A + "r2/rows/%s.cpp" % a, encoding="utf-8").read()
        osrc_text = (nf.R / "rebuild/src/compiled" / osrc).read_text(encoding="utf-8")
        fl = nf.flags_of(owner, osrc_text)
        retail = bytes.fromhex(nf.catalog()[a]["bytes"])
        while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]
        wd = nf.W / "g" / (a + "_r2" + tag); wd.mkdir(parents=True, exist_ok=True)
        leaf = nf.leaf_of(src, a)
        st, built, sec = V.parity_of(V.vc71(src), a, leaf, retail, wd, V.env(), extra=tuple(fl))
        obj = wd / (a + ".obj")
        if obj.exists(): shutil.copy(obj, od / (a + ".obj"))
        print("%s %s leaf=%s owner=%s flags=%s obj=%s" % (a, st, leaf, owner, " ".join(fl), obj.exists()), file=log); log.flush()
        subprocess.run(["python3", str(nf.R / "tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py"), str(nf.A), str(wd)], capture_output=True)
    except Exception as ex:
        print("ERR", a, repr(ex)[:300], file=log); log.flush()
print("CC_DONE", file=log); log.close()
