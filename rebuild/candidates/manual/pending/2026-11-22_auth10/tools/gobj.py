"""gobj.py <addr> <obj> <leaf>: grade an already-built obj like q.py (prints q.py-style output)."""
import sys, os, csv, glob
csv.field_size_limit(10**8)
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = Path(os.environ["S"]); addr = sys.argv[1].lower().replace("0x",""); obj = Path(sys.argv[2]); leaf = sys.argv[3]
retail = None
for f in ["/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv"] + glob.glob(str(S/"batch"/"*_oracle.tsv")) + glob.glob("/home/user/FableDecomp/rebuild/candidates/manual/batches/*/oracle.tsv"):
    for r in csv.DictReader(open(f, encoding="utf-8-sig"), delimiter="\t"):
        if r["address"].lower().replace("0x","") == addr: retail = bytes.fromhex(r["bytes"]); break
    if retail: break
while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]
res = V.obj_text(obj, leaf); built, sec = res[0], res[1]; roff = res[3] if len(res) > 3 else 0
rel = [r-roff for r in V.obj_relocs(obj, sec) if r >= roff]
st = "MATCH" if retail == built else ("RELOCATION_MATCH" if V.mask(retail, rel) == V.mask(built, rel) else f"DIFFER({len(built)}v{len(retail)})")
print(addr, "obj", st)
md = Cs(CS_ARCH_X86, CS_MODE_32)
dis = lambda b, base: [f"{i.address:08x}  {i.bytes.hex():22} {i.mnemonic} {i.op_str}" for i in md.disasm(b, base)]
print("--- retail (%d bytes)" % len(retail)); print("\n".join(dis(retail, int(addr,16))))
print("--- built (%d bytes)" % len(built)); print("\n".join(dis(built, int(addr,16))))
