#!/usr/bin/env python3
"""Quick parity harness: q.py <addr> <src.cpp> [leaf] [--flags /GS ...]
Compiles under the verifier's base flags (+extra), compares against the oracle bytes
found in the batch TSVs, prints side-by-side capstone disassembly when they differ."""
import sys, os, csv, glob
from pathlib import Path
sys.path.insert(0, "/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad/mig5/repo/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = Path(os.environ["S"])
addr = sys.argv[1].lower().replace("0x","")
src = Path(sys.argv[2]).read_text(encoding="utf-8")
args = sys.argv[3:]
extra = []
if "--flags" in args:
    i = args.index("--flags"); extra = args[i+1:]; args = args[:i]
leaf = args[0] if args else None
retail = None; name = None
for f in ["/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad/mig5/repo/rebuild/oracles/auto-re-candidates.tsv"] + list(glob.glob(str(S/"batch"/"*_oracle.tsv"))) + list(glob.glob("/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad/mig5/repo/rebuild/candidates/manual/batches/*/oracle.tsv")):   # catalog oracle first (authoritative for landed rows)
    for r in csv.DictReader(open(f, encoding="utf-8-sig"), delimiter="\t"):
        if r["address"].lower().replace("0x","") == addr:
            retail = bytes.fromhex(r["bytes"]); name = r["name"]; break
    if retail: break
if retail is None: sys.exit("no oracle for " + addr)
while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]   # trailing int3/nop padding is not part of the function
leaf = leaf or name.rsplit("::",1)[-1]
work = S/"mig5"/"work"/"q"/addr; work.mkdir(parents=True, exist_ok=True)
e = V.env()
st, built, sec = V.parity_of(V.vc71(src), addr, leaf, retail, work, e, extra=tuple(extra))
print(addr, name, st, "flags=", extra)
if st == "SRC_FAIL":
    cp = V.cl(["/nologo","/c"]+V.BASE_FLAGS+extra+[f"/Fo{work/(addr+'.obj')}", str(work/(addr+'.cpp'))], e)
    print(cp.stdout[-3000:], cp.stderr[-2000:])
elif not st.startswith(("MATCH","RELOCATION")) or "-v" in sys.argv:
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    def dis(b, base):
        return [f"{i.address:08x}  {i.bytes.hex():22} {i.mnemonic} {i.op_str}" for i in md.disasm(b, base)]
    a = dis(retail, int(addr,16)); b = dis(built or b"", int(addr,16))
    print("--- retail (%d bytes)" % len(retail)); print("\n".join(a))
    print("--- built (%d bytes)" % len(built or b"")); print("\n".join(b))
