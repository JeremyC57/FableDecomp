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
extra=[]; leaf=None; full = "--full" in args
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
st, built, sec = V.parity_of(V.vc71(src), addr, leaf, retail, work, e, extra=tuple(extra))
print(addr, name, st, "flags=", extra)
if 0: print(f"{time.strftime('%H:%M')}\t{addr}\t{st}\t{idea}\t{srcp.name}\n")
if st == "SRC_FAIL" and 0:
    cp = V.cl(["/nologo","/c"]+V.BASE_FLAGS+extra+[f"/Fo{work/(addr+'.obj')}", str(work/(addr+'.cpp'))], e)
    print(cp.stdout[-3000:], cp.stderr[-2000:]); sys.exit()
pass
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
lo=int(os.environ.get("LO","0"),16); hi=int(os.environ.get("HI","ffffffff"),16)
A=[x for x in a if lo<=x[0]<hi]; B=[x for x in b if lo<=x[0]<hi]
for k in range(max(len(A),len(B))):
    l=A[k] if k<len(A) else (0,""); r=B[k] if k<len(B) else (0,"")
    print(f"{l[0]:08x} {l[1]:<34} {'  ' if l[1]==r[1] else '<>'} {r[1]}")
if os.environ.get("ORD"):
    def order(ins):
        out=[];cur=[]
        for i,x in enumerate(ins):
            if x.startswith("fld dword ptr"):
                y=next(z for z in ins[i+1:i+5] if z.startswith("fmul"))
                m=re.search(r"\[ecx(?: \+ (0x[0-9a-f]+))?\]", x+" "+y)
                off=int(m.group(1),16) if (m and m.group(1)) else 0
                cur.append("%d%s"%(off//16, "b" if "ecx" in x else "a"))
            if x.startswith("fstp"):
                out.append(" ".join(cur)); cur=[]
        return out
    print("retail:", " | ".join(order([x[1] for x in A])))
    print("ours:  ", " | ".join(order([x[1] for x in B])))
