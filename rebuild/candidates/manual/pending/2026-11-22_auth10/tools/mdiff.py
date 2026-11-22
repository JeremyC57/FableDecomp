"""mdiff.py <addr> <obj> <leaf> [ctx]: reloc-masked instruction diff (only real differences), with counts."""
import sys, os, csv, glob, difflib
csv.field_size_limit(10**8)
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = Path(os.environ["S"]); addr = sys.argv[1].lower().replace("0x",""); obj = Path(sys.argv[2]); leaf = sys.argv[3]; ctx = int(sys.argv[4]) if len(sys.argv) > 4 else 1
retail = None
for f in ["/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv"] + glob.glob(str(S/"batch"/"*_oracle.tsv")) + glob.glob("/home/user/FableDecomp/rebuild/candidates/manual/batches/*/oracle.tsv"):
    for r in csv.DictReader(open(f, encoding="utf-8-sig"), delimiter="\t"):
        if r["address"].lower().replace("0x","") == addr: retail = bytes.fromhex(r["bytes"]); break
    if retail: break
while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]
res = V.obj_text(obj, leaf); built, sec = res[0], res[1]; roff = res[3] if len(res) > 3 else 0
rel = [r-roff for r in V.obj_relocs(obj, sec) if r >= roff]
md = Cs(CS_ARCH_X86, CS_MODE_32)
import re
J = ("j", "loop")
def ins(b, relset):
    out = []
    for i in md.disasm(b, 0):
        hit = any(i.address <= r < i.address + i.size for r in relset)
        t = f"{i.mnemonic} {i.op_str}"
        if i.mnemonic.startswith(J) or i.mnemonic == "call": t = i.mnemonic + " <J>"
        elif hit: t = re.sub(r"(?<![\w])(0x[0-9a-f]+|0)(?![\w])", "<A>", t)
        out.append((i.address, t))
    return out
def rins(b):
    out = []
    for i in md.disasm(b, int(addr,16)):
        t = f"{i.mnemonic} {i.op_str}"
        if i.mnemonic.startswith(J) or i.mnemonic == "call": t = i.mnemonic + " <J>"
        else: t = re.sub(r"0x[0-9a-f]{6,}", "<A>", t)
        out.append((i.address - int(addr,16), t))
    return out
A = rins(retail); B = ins(built, set(rel))
a = [t for _, t in A]; b = [t for _, t in B]
sm = difflib.SequenceMatcher(None, a, b, autojunk=False); nd = 0
for op, i1, i2, j1, j2 in sm.get_opcodes():
    if op == "equal": continue
    nd += max(i2-i1, j2-j1)
    print(f"@@ r{A[i1][0] if i1 < len(A) else len(retail):x} b{B[j1][0] if j1 < len(B) else len(built):x}")
    for k in range(i1, i2): print("-", a[k])
    for k in range(j1, j2): print("+", b[k])
print("DIFFINS", nd, "len", len(built), "v", len(retail))
