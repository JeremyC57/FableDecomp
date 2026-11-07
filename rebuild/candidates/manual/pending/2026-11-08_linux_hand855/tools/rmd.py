# rmd.py <addr> <leaf> [show]: reloc-masked instruction diff of the last q.py build of <addr> (QTAG-aware work dir)
import sys,os,csv,glob,difflib,re
sys.path.insert(0,"/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from pathlib import Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S=os.environ["S"]; a=sys.argv[1]; leaf=sys.argv[2]
obj=Path(f"{S}/wine/q/{a}/{a}.obj")
rb=None
for f in glob.glob(S+"/batch/chunk855_oracle.tsv"):
    for r in csv.DictReader(open(f,encoding="utf-8-sig"),delimiter="\t"):
        if r["address"].lower()==a: rb=bytes.fromhex(r["bytes"])
while rb and rb[-1] in (0xcc,0x90): rb=rb[:-1]
res=V.obj_text(obj,leaf); built,sec=res[0],res[1]; roff=res[3] if len(res)>3 else 0
rel=[r-roff for r in V.obj_relocs(obj,sec) if r>=roff]
ma,mb=V.mask(rb,rel),V.mask(built,rel)
# retail relocation positions are unknown; mask retail at the same instruction-relative spots is not possible when shifted,
# so mask every 4-byte operand that looks like an absolute address (>=0x400000) in retail, and every reloc in built.
md=Cs(CS_ARCH_X86,CS_MODE_32)
base=int(a,16)
def norm(ins):
    s=ins.mnemonic+" "+ins.op_str
    s=re.sub(r"0x[0-9a-f]{6,}","G",s)
    return s
def dis(b,relocs):
    out=[]
    for i in md.disasm(b,base):
        s=i.mnemonic+" "+i.op_str
        off=i.address-base
        if any(off<=r<off+i.size for r in relocs):
            s=re.sub(r"(0x[0-9a-f]+|\b0\b)(?=\]|$)","G",s) if not i.mnemonic.startswith(("call","j")) else i.mnemonic+" T"
        else:
            s=re.sub(r"0x[0-9a-f]{6,}","G",s)
            if i.mnemonic.startswith(("call","j")) and "ptr" not in s: s=re.sub(r"G$",lambda m:"L%x"%(int(i.op_str,16)-base) if i.op_str.startswith("0x") else "G",s) if False else s
        out.append(s)
    return out
# retail: relocs unknown -> treat any operand >= 0x400000 as G
R=[]
for i in md.disasm(rb,base):
    s=i.mnemonic+" "+i.op_str
    if i.mnemonic=="call": s="call T"
    s=re.sub(r"0x[0-9a-f]{6,}","G",s); R.append(s)
B=[]
for i in md.disasm(built,base):
    s=i.mnemonic+" "+i.op_str
    off=i.address-base
    if i.mnemonic=="call": s="call T"
    elif any(off<=r<off+i.size for r in rel):
        s=re.sub(r"(?<![\w+*-])0x[0-9a-f]+(?=\]|$)|(?<=[\[ ,])0(?=\]|$)","G",s); s=re.sub(r"0x[0-9a-f]{6,}","G",s)
        pass
    else: s=re.sub(r"0x[0-9a-f]{6,}","G",s)
    B.append(s)
R=[re.sub(r" \+ G\]","]",x) for x in R]; B=[re.sub(r" \+ G\]","]",x) for x in B]
R=[re.sub(r"\[G\]","[G]",x) for x in R]
d=list(difflib.unified_diff(R,B,lineterm="",n=1))
n=sum(1 for l in d if l[:1] in "+-" and l[:3] not in ("---","+++"))
if len(sys.argv)>3: print("\n".join(d))
print("RMD",n,len(built),len(rb))
