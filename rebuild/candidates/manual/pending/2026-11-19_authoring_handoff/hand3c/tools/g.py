#!/usr/bin/env python3
"""g.py <addr> <src.cpp> "<idea>" [--leaf L] [--flags ...] [--full] : grade + masked unified diff + ATTEMPTS.log line"""
import sys, os, csv, glob, re, difflib, time
csv.field_size_limit(10**8)
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = Path(os.environ["S"]); W = Path(os.environ["W"])
args = sys.argv[1:]
addr = args[0].lower().replace("0x",""); srcp = Path(args[1]); idea = args[2] if len(args)>2 else ""
extra=[]; leaf=None; full = "--full" in args; qfe = "--qfe" in args
if qfe: args=[a for a in args if a!="--qfe"]
if "--flags" in args: i=args.index("--flags"); extra=args[i+1:]; args=args[:i]
if "--leaf" in args: leaf=args[args.index("--leaf")+1]
src = srcp.read_text(encoding="utf-8")
m = re.search(r"^// Leaf:\s*(\S+)", src, re.M)
if not leaf and m: leaf = m.group(1)
m = re.search(r"^// Flags:\s*(.*)$", src, re.M)
if m and not extra and m.group(1).strip().lower() not in ("none",""): extra = m.group(1).split()
retail=None; name=None; length=None
for f in [str(W/"oracle_override.tsv")] + [str(S/"batch"/"hand2c_oracle.tsv")]:
    if not os.path.exists(f): continue
    for r in csv.DictReader(open(f, encoding="utf-8-sig"), delimiter="\t"):
        if r["address"].lower().replace("0x","") == addr:
            retail = bytes.fromhex(r["bytes"]); name=r["name"]; length=int(r.get("length") or 0); break
    if retail: break
if length and len(retail) > length: retail = retail[:length]
while retail and retail[-1] in (0xCC,0x90): retail=retail[:-1]
leaf = leaf or name.rsplit("::",1)[-1]
work = W/"tmp"/"q"/addr; work.mkdir(parents=True, exist_ok=True)
e = V.env()
if qfe: V.use_qfe(); e = V.env()
st, built, sec = V.parity_of(V.vc71(src), addr, leaf, retail, work, e, extra=tuple(extra))
print(addr, name, st, "flags=", extra)
with open(W/"ATTEMPTS.log","a") as fh: fh.write(f"{time.strftime('%H:%M')}\t{addr}\t{st}\t{idea}\t{srcp.name}\n")
if st == "SRC_FAIL":
    cp = V.cl(["/nologo","/c"]+V.BASE_FLAGS+extra+[f"/Fo{work/(addr+'.obj')}", str(work/(addr+'.cpp'))], e)
    print(cp.stdout[-3000:], cp.stderr[-2000:]); sys.exit()
if st.startswith(("MATCH","RELOCATION")) and not full: sys.exit()
md = Cs(CS_ARCH_X86, CS_MODE_32)
def dis(b, base):
    out=[]
    for i in md.disasm(b, base):
        op = i.op_str
        if (i.mnemonic in ("call","jmp") or i.mnemonic.startswith("j")) and re.fullmatch(r"0x[0-9a-f]+", op):
            op = re.sub(r"0x[0-9a-f]+", lambda mm: ("L%x"%(int(mm.group(0),16)-base)) if base<=int(mm.group(0),16)<base+len(b) else "EXT", op)
        op = re.sub(r"0x[0-9a-f]{6,}", "ABS", op)
        op = re.sub(r"\[0\]", "[ABS]", op); op = re.sub(r"\[(\w+\*\d)\]", r"[\1 + ABS]", op); op = re.sub(r"(\[[^\]]*\+ )0\]", r"\1ABS]", op)
        if i.mnemonic=="call" and op==("L%x"%(i.address+i.size-base)): op="EXT"
        if i.mnemonic in ("mov","push") and (op.endswith(", ABS") or op=="ABS" or ((op.endswith(", 0") or op=="0") and i.size>=5 and i.bytes[-4:]==b"\0\0\0\0")): op=re.sub(r"(ABS|0)$","IMM",op)
        if i.mnemonic in ("push","mov") and op.endswith(", 0") and False: pass
        out.append((i.address, f"{i.mnemonic} {op}"))
    return out
a = dis(retail, int(addr,16)); b = dis(built or b"", int(addr,16))
print(f"retail {len(retail)} built {len(built or b'')}")
sm = difflib.SequenceMatcher(None, [x[1] for x in a], [x[1] for x in b], autojunk=False)
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=="equal":
        if full:
            for k in range(i1,i2): print(f"  {a[k][0]:08x} {a[k][1]}")
        continue
    print(f"@@ retail {a[i1][0] if i1<len(a) else 0:08x} ours {b[j1][0] if j1<len(b) else 0:08x}")
    for k in range(max(0,i1-2), i1): print(f"  {a[k][1]}")
    for k in range(i1,i2): print(f"- {a[k][0]:08x} {a[k][1]}")
    for k in range(j1,j2): print(f"+ {b[k][0]:08x} {b[k][1]}")
