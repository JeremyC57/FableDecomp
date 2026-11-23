#!/usr/bin/env python3
"""qq.py <addr> <src.cpp> [leaf] [--flags ...]: the repo lane_tools/q.py grade without capstone (status line only; no disassembly).
Work dir $S/wine/q/<addr> (point S at a folder inside the agent dir). Oracle: rebuild/oracles/auto-re-candidates.tsv."""
import sys, os, csv
csv.field_size_limit(10**8)
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
S = Path(os.environ["S"])
addr = sys.argv[1].lower().replace("0x","")
src = Path(sys.argv[2]).read_text(encoding="utf-8")
args = sys.argv[3:]; extra = []
if "--flags" in args:
    i = args.index("--flags"); extra = args[i+1:]; args = args[:i]
leaf = args[0] if args else None
retail = None; name = None
for r in csv.DictReader(open("/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv", encoding="utf-8-sig"), delimiter="\t"):
    if r["address"].lower().replace("0x","") == addr:
        retail = bytes.fromhex(r["bytes"]); name = r["name"]; break
if retail is None: sys.exit("no oracle for " + addr)
while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]
leaf = leaf or name.rsplit("::",1)[-1]
work = S/"wine"/"q"/addr; work.mkdir(parents=True, exist_ok=True)
e = V.env()
st, built, sec = V.parity_of(V.vc71(src), addr, leaf, retail, work, e, extra=tuple(extra))
print(addr, name, st, "flags=", extra)
