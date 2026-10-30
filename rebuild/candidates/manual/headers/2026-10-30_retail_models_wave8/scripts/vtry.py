#!/usr/bin/env python3
"""vtry.py <addr> <src.cpp>: compile a scratch source under the row's cached mcheck variant and grade it against retail
(MATCH / RELOCATION_MATCH / DIFFER with sizes). For experiments only (no worktree writes)."""
import sys, os, json, re, subprocess
from pathlib import Path
ROOT = Path(os.environ["FABLE_ROOT"]); os.chdir(ROOT); S = Path(os.environ["S"])
sys.path.insert(0, str(ROOT / "tools/decomp_pipeline"))
import verify_and_land as V
a, src = sys.argv[1].lower(), sys.argv[2]
cat = {l.split("\t")[0].lower(): l.split("\t") for l in open("rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]}
retail = bytes.fromhex(cat[a][3])
while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]
var = json.load(open(S / "mig8/work/mc_variant.json")).get(a, [[], ""])
text = open(src).read()
m = re.search(r"^//\s*Leaf:\s*(\S+)", text, re.M); leaf = m.group(1) if m else sys.argv[3]
w = S / "mig8/work/vtry" / a; w.mkdir(parents=True, exist_ok=True)
sp = w / "t.cpp"; sp.write_text(V.vc71((var[1] + "\n" + text) if var[1] else text)); obj = w / "t.obj"
if obj.exists(): obj.unlink()
fl = list(var[0]); 
cp = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + fl + ["/Oy-"] * 0 + [f"/Fo{obj}", str(sp)], V.env())
if cp.returncode or not obj.exists(): print("SRC_FAIL", "\n".join(l for l in cp.stdout.splitlines() if "error" in l)[:800]); sys.exit()
b, sec, sym = V.obj_text(obj, leaf)
print("MATCH" if b == retail else "DIFFER", len(b), "v", len(retail), sym)
open(w / "new.bin", "wb").write(b); open(w / "retail.bin", "wb").write(retail)
