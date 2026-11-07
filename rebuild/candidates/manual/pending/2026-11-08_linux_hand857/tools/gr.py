#!/usr/bin/env python3
"""gr.py <addr> <src.cpp> [tag] [--note TEXT] [--extra FLAGS...]
Grade one source (leaf + flags from its // Leaf: / // Flags: lines), overlay = agent857/hdr.
Prints status, md (jump/call/addr-masked disasm diff lines) and masked-byte diff count,
writes the side-by-side diff to tmp/<addr>.<tag>.diff and appends to tmp/attempts.log."""
import sys, os, re, csv, difflib, time
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
S = Path(os.environ["S"]); A = S/"w22"/"agent857"
os.environ["FABLE_HDR_OVERLAY"] = str(A/"hdr")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
args = sys.argv[1:]
note = ""
if "--note" in args:
    i = args.index("--note"); note = args[i+1]; del args[i:i+2]
xflags = []
if "--extra" in args:
    i = args.index("--extra"); xflags = args[i+1:]; args = args[:i]
args=[x for x in args if x!='--qfe']
addr = args[0].lower(); srcp = Path(args[1]); tag = args[2] if len(args) > 2 else "x"
src = srcp.read_text(encoding="utf-8")
m = re.search(r"^// Leaf: *(\S+)", src, re.M); leaf = m.group(1) if m else None
m = re.search(r"^// Flags: *(.*)$", src, re.M); flags = m.group(1).split() if m else []
flags += xflags
for r in csv.DictReader(open(S/"batch"/"chunk857_oracle.tsv", encoding="utf-8-sig"), delimiter="\t"):
    if r["address"].lower().replace("0x", "") == addr: retail = bytes.fromhex(r["bytes"]); name = r["name"]
while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]
leaf = leaf or name.rsplit("::", 1)[-1]
work = A/"w"/(addr + "_" + tag); work.mkdir(parents=True, exist_ok=True)
if "--qfe" in sys.argv: V.use_qfe()
e = V.env()
t0 = time.time()
st, built, sec = V.parity_of(V.vc71(src), addr, leaf, retail, work, e, extra=tuple(flags))
out = []
md = -1; nb = -1; first = ""
if st == "SRC_FAIL":
    cp = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + flags + [f"/Fo{work/(addr+'.obj')}", str(work/(addr+'.cpp'))], e)
    print(cp.stdout[-2500:], cp.stderr[-1500:])
elif built:
    obj = work/(addr + ".obj")
    rel = V.obj_relocs(obj, sec)
    ma, mb = V.mask(retail, rel), V.mask(built, rel)
    nb = sum(1 for i in range(min(len(ma), len(mb))) if ma[i] != mb[i]) + abs(len(ma) - len(mb))
    cs = Cs(CS_ARCH_X86, CS_MODE_32)
    base = int(addr, 16)
    def dis(b):
        return [(i.address, i.mnemonic + " " + i.op_str) for i in cs.disasm(b, base)]
    def norm(s):
        s = re.sub(r"0x[0-9a-f]{5,}", "ADDR", s)
        s = re.sub(r"^(call|j[a-z]+) \S+", r"\1 X", s)
        return s
    bp = bytearray(built)
    for o in rel:
        if o + 4 <= len(bp): bp[o:o+4] = (0x00ABCDEF).to_bytes(4, "little")
    ra, ba = dis(retail), dis(bytes(bp))
    rn = [norm(x[1]) for x in ra]; bn = [norm(x[1]) for x in ba]
    sm = difflib.SequenceMatcher(None, rn, bn, autojunk=False)
    md = 0
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op == "equal":
            out.append(f"  == {i2-i1} same (retail {ra[i1][0]:08x}.., ours {ba[j1][0]:08x}..)"); continue
        for k in range(i1, i2): out.append(f"  - {ra[k][0]:08x} {ra[k][1]}"); md += 1
        for k in range(j1, j2): out.append(f"  + {ba[k][0]:08x} {ba[k][1]}"); md += 1
    if md == 0 and nb:
        d = [i for i in range(min(len(ma), len(mb))) if ma[i] != mb[i]]
        out.append("masked byte diffs at " + str([hex(base + i) for i in d[:20]])); first = " at " + ",".join(hex(base + i) for i in d[:4])
        # show the jumps around them
        for a_, s_ in ra:
            if any(a_ <= base + i < a_ + 8 for i in d[:6]): out.append(f"  R {a_:08x} {s_}")
        for a_, s_ in ba:
            if any(a_ <= base + i < a_ + 8 for i in d[:6]): out.append(f"  O {a_:08x} {s_}")
    (A/"tmp"/f"{addr}.{tag}.diff").write_text("\n".join(out) + "\n")
line = f"{time.strftime('%H:%M:%S')}\t{addr}\t{tag}\t{st}\tmd={md}\tbytes={nb}{first}\t{int(time.time()-t0)}s\t{note}"
print(line)
with open(A/"tmp"/"attempts.log", "a") as f: f.write(line + "\n")
if "-v" in sys.argv or (md and md < 80 and "-q" not in sys.argv):
    print("\n".join(out[:200]))
