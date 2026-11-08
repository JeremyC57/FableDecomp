#!/usr/bin/env python3
"""gr.py <src.cpp> [tag] [--note TEXT] [--extra FLAGS...] [-v]
Research861 grader: address = first 8 chars of the basename; leaf/flags from // Leaf: / // Flags:; overlay by address
(chunk 856 / 860 rows); oracle from any $S/batch/*_oracle.tsv. Prints status, md (reloc/jump-masked disasm diff lines,
chunk-859 metric) and masked-byte count; side-by-side diff in tmp/<addr>.<tag>.diff; appends to tmp/attempts.log."""
import sys, os, re, csv, difflib, time, glob
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
S = Path(os.environ["S"]); A = S/"w22"/"agent862"
args = sys.argv[1:]
note = ""
if "--note" in args:
    i = args.index("--note"); note = args[i+1]; del args[i:i+2]
xflags = []
if "--extra" in args:
    i = args.index("--extra"); xflags = args[i+1:]; args = args[:i]
verbose = "-v" in args; args = [x for x in args if x != "-v"]
srcp = Path(args[0]); tag = args[1] if len(args) > 1 else "x"
addr = srcp.name[:8].lower()
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
src = srcp.read_text(encoding="utf-8")
m = re.search(r"^// Leaf: *(\S+)", src, re.M); leaf = m.group(1) if m else None
m = re.search(r"^// Flags: *(.*)$", src, re.M); flags = [x for x in (m.group(1).split() if m else []) if x != "none"]
flags += xflags
retail = None
for f in [str(S/"batch"/"chunk862_oracle.tsv")]:
    for r in csv.DictReader(open(f, encoding="utf-8-sig"), delimiter="\t"):
        if r["address"].lower().replace("0x", "") == addr: retail = bytes.fromhex(r["bytes"]); name = r["name"]; break
    if retail: break
if retail is None: sys.exit("no oracle for " + addr)
while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]
CUT = {}
if addr in CUT: retail = retail[:CUT[addr]]
leaf = leaf or name.rsplit("::", 1)[-1]
work = A/"work"/("g_" + addr + "_" + tag); work.mkdir(parents=True, exist_ok=True)
e = V.env()
t0 = time.time()
st, built, sec = V.parity_of(V.vc71(src), addr, leaf, retail, work, e, extra=tuple(flags))
out = []; md = -1; nb = -1; first = ""
if st == "SRC_FAIL":
    cp = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + flags + [f"/Fo{work/(addr+'.obj')}", str(work/(addr+'.cpp'))], e)
    print(cp.stdout[-2500:], cp.stderr[-1500:])
elif built:
    obj = work/(addr + ".obj")
    rel = V.obj_relocs(obj, sec)
    ma, mb = V.mask(retail, rel), V.mask(built, rel)
    nb = sum(1 for i in range(min(len(ma), len(mb))) if ma[i] != mb[i]) + abs(len(ma) - len(mb))
    cs = Cs(CS_ARCH_X86, CS_MODE_32); base = int(addr, 16)
    def dis(b): return [(i.address, i.mnemonic + " " + i.op_str) for i in cs.disasm(b, base)]
    def norm(s):
        s = re.sub(r"0x[0-9a-f]{5,}", "ADDR", s)
        s = re.sub(r"^(call|j[a-z]+) \S+", r"\1 X", s)
        return s
    bp = bytearray(built)
    for o in rel:
        if o + 4 <= len(bp): bp[o:o+4] = (0x00ABCDEF).to_bytes(4, "little")
    ra, ba = dis(retail), dis(bytes(bp))
    rn = [norm(x[1]) for x in ra]; bn = [norm(x[1]) for x in ba]
    sm = difflib.SequenceMatcher(None, rn, bn, autojunk=False); md = 0
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op == "equal":
            out.append(f"  == {i2-i1} same (retail {ra[i1][0]:08x}.., ours {ba[j1][0]:08x}..)"); continue
        for k in range(i1, i2): out.append(f"  - {ra[k][0]:08x} {ra[k][1]}"); md += 1
        for k in range(j1, j2): out.append(f"  + {ba[k][0]:08x} {ba[k][1]}"); md += 1
    if md == 0 and nb:
        d = [i for i in range(min(len(ma), len(mb))) if ma[i] != mb[i]]
        out.append("masked byte diffs at " + str([hex(base + i) for i in d[:20]])); first = " at " + ",".join(hex(base + i) for i in d[:4])
    (A/"tmp"/f"{addr}.{tag}.diff").write_text("\n".join(out) + "\n")
line = f"{time.strftime('%H:%M:%S')}\t{addr}\t{tag}\t{st}\tmd={md}\tbytes={nb}{first}\tflags={' '.join(flags)}\t{int(time.time()-t0)}s\t{note}"
print(line)
with open(A/"tmp"/"attempts.log", "a") as f: f.write(line + "\n")
if verbose or (md and 0 < md < 60): print("\n".join(out[:120]))
