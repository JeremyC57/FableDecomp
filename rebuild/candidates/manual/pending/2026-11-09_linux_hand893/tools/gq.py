#!/usr/bin/env python3
"""gq.py <addr> <src.cpp> [tag] [--flags ...] [--leaf L] [--nolog]
Grade one source like q.py (verifier base flags; honours the source's // Leaf: and // Flags: lines),
mask relocations in BOTH images, report status + md (masked differing instruction lines) + first
differing instruction index; write the side-by-side masked diff to out/<addr>.<tag>.txt and append
one line to ATTEMPTS.log."""
import sys, os, csv, re, difflib, time
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = Path(os.environ["S"]); A = S/"w22"/"agent893"
args = sys.argv[1:]
nolog = "--nolog" in args; args = [a for a in args if a != "--nolog"]
extra = None
if "--flags" in args:
    i = args.index("--flags"); extra = args[i+1:]; args = args[:i]
leaf = None
if "--leaf" in args:
    i = args.index("--leaf"); leaf = args[i+1]; del args[i:i+2]
addr = args[0].lower().replace("0x", ""); srcp = Path(args[1]); tag = args[2] if len(args) > 2 else srcp.stem
src = srcp.read_text(encoding="utf-8")
if leaf is None:
    m = re.search(r"^// Leaf: (\S+)\s*$", src, re.M); leaf = m.group(1) if m else None
if extra is None:
    m = re.search(r"^// Flags: (.+)$", src, re.M); extra = m.group(1).split() if m else []
retail = None; name = None
for r in csv.DictReader(open(S/"batch"/"chunk893_oracle.tsv", encoding="utf-8-sig"), delimiter="\t"):
    if r["address"].lower().replace("0x", "") == addr:
        retail = bytes.fromhex(r["bytes"]); name = r["name"]; break
while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]
leaf = leaf or name.rsplit("::", 1)[-1]
work = A/"wk"/addr; work.mkdir(parents=True, exist_ok=True)
e = V.env()
t0 = time.time()
st, built, sec = V.parity_of(V.vc71(src), addr, leaf, retail, work, e, extra=tuple(extra))
dt = time.time() - t0
md = -1; first = -1; out = []
if st == "SRC_FAIL":
    cp = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + extra + [f"/Fo{work/(addr+'.obj')}", str(work/(addr+'.cpp'))], e)
    out.append(cp.stdout[-4000:] + cp.stderr[-2000:])
elif built is not None:
    rel = V.obj_relocs(work/(addr+'.obj'), sec)
    def mk(b):
        b = bytearray(b)
        for r in rel:
            for k in range(4):
                if r+k < len(b): b[r+k] = 0
        return bytes(b)
    cs = Cs(CS_ARCH_X86, CS_MODE_32)
    base = int(addr, 16)
    def dis(b):
        return [f"{i.mnemonic} {i.op_str}" for i in cs.disasm(b, base)], [i.address for i in cs.disasm(b, base)]
    R, Ra = dis(mk(retail)); B, Ba = dis(mk(built))
    # normalise branch targets to relative-instruction ids is overkill; jumps compare by address (same base)
    sm = difflib.SequenceMatcher(None, R, B, autojunk=False)
    md = 0
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op != "equal":
            md += max(i2-i1, j2-j1)
            if first < 0: first = i1
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op == "equal":
            for k in range(i2-i1): out.append(f"  {Ra[i1+k]:08x} {R[i1+k]:45} | {B[j1+k]}")
        else:
            n = max(i2-i1, j2-j1)
            for k in range(n):
                l = f"{Ra[i1+k]:08x} {R[i1+k]}" if i1+k < i2 else ""
                r_ = B[j1+k] if j1+k < j2 else ""
                out.append(f"* {l:54} | {r_}")
line = f"{time.strftime('%H:%M:%S')}\t{addr}\t{tag}\t{st}\tmd={md}\tfirst={first}\t{dt:.0f}s\tflags={' '.join(extra)}\tleaf={leaf}\tsrc={srcp}"
print(line)
(A/"out").mkdir(exist_ok=True)
(A/"out"/f"{addr}.{tag}.txt").write_text(line + "\n" + "\n".join(out) + "\n")
if not nolog:
    with open(A/"ATTEMPTS.log", "a") as f: f.write(line + "\n")
