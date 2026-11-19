# probe harness (no ATTEMPTS logging): pp.py file.cpp ... -> number of differing insns vs retail
import sys, os, csv, re
csv.field_size_limit(10**8)
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
W=Path(os.environ["W"]); S=Path(os.environ["S"])
addr="00c40590"; LEAF=os.environ.get("LEAF","NormalizeOrientationFrame")
for r in csv.DictReader(open(S/"batch"/"hand2c_oracle.tsv", encoding="utf-8-sig"), delimiter="\t"):
    if r["address"].lower().replace("0x","")==addr: retail=bytes.fromhex(r["bytes"]); L=int(r.get("length") or 0); break
retail=retail[:L]
md=Cs(CS_ARCH_X86,CS_MODE_32)
def norm(b):
    return [re.sub(r"0x[0-9a-f]{5,}","A",i.mnemonic+" "+i.op_str).replace("[0]","[A]") for i in md.disasm(b,0)]
R=norm(retail)
e=V.env()
for f in sys.argv[1:]:
    src=open(f).read(); work=W/"probe"/"c40"/"q"; work.mkdir(parents=True,exist_ok=True)
    st,built,sec=V.parity_of(V.vc71(src),addr,sys.argv[0] and LEAF,retail,work,e,extra=())
    B=norm(built or b"")
    d=sum(1 for a,b in zip(R,B) if a!=b)+abs(len(R)-len(B))
    print(os.path.basename(f), st, d, ' '.join('%d'%k for k,(a,b) in enumerate(zip(R,B)) if a!=b and k>=40))
