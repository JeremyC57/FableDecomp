"""objcmp.py <addr8> <obj> <leaf>: grade a kept object against the oracle like parity_of (MATCH / RELOCATION_MATCH / DIFFER + differing insns)."""
import sys, csv, glob, os
csv.field_size_limit(10**8)
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
addr, obj, leaf = sys.argv[1:4]
S = os.environ["S"]; retail = None
for f in ["/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv"] + glob.glob(S + "/batch/*_oracle.tsv"):
    for r in csv.DictReader(open(f, encoding="utf-8-sig"), delimiter="\t"):
        if r["address"].lower().replace("0x", "") == addr: retail = bytes.fromhex(r["bytes"]); break
    if retail: break
while retail[-1] in (0xCC, 0x90): retail = retail[:-1]
res = V.obj_text(obj, leaf); built, sec = res[0], res[1]
rel = V.obj_relocs(obj, sec)
st = "MATCH" if built == retail else ("RELOCATION_MATCH" if V.mask(retail, rel) == V.mask(built, rel) else "DIFFER(%dv%d)" % (len(built), len(retail)))
print(st)
if not st.endswith("MATCH"):
    md = Cs(CS_ARCH_X86, CS_MODE_32); base = int(addr, 16)
    a = [(i.address, i.mnemonic + " " + i.op_str) for i in md.disasm(retail, base)]
    b = [(i.address, i.mnemonic + " " + i.op_str) for i in md.disasm(built, base)]
    for x, y in zip(a, b):
        if x[1] != y[1] and not ("0x" in x[1] and x[1].split()[0] == y[1].split()[0] and x[1].split()[0] in ("call", "jmp", "push", "mov")): print("%08x  - %-40s + %s" % (x[0], x[1], y[1]))
