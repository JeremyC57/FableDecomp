# nofr.py <addr> <tag>: re-diff the last grade with esp displacements masked (structure-only view)
import sys,os,re,difflib,csv
sys.path.insert(0,"/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
S=os.environ["S"]; A=S+"/w22/agent862"; a=sys.argv[1]; t=sys.argv[2]
obj=f"{A}/work/g_{a}_{t}/{a}.obj"
src=open(f"{A}/work/g_{a}_{t}/{a}.cpp").read()
leaf=re.search(r"^// Leaf: *(\S+)",src,re.M).group(1)
res=V.obj_text(__import__('pathlib').Path(obj),leaf); built,sec=res[0],res[1]
for r in csv.DictReader(open(S+"/batch/chunk862_oracle.tsv",encoding="utf-8-sig"),delimiter="\t"):
    if r["address"]==a: rb=bytes.fromhex(r["bytes"])
cs=Cs(CS_ARCH_X86,CS_MODE_32); base=int(a,16)
def norm(s):
    s=re.sub(r"0x[0-9a-f]{5,}","ADDR",s); s=re.sub(r"^(call|j[a-z]+) \S+",r"\1 X",s)
    s=re.sub(r"\[esp \+ 0x[0-9a-f]+\]","[esp+N]",s); s=re.sub(r"esp, 0x[0-9a-f]+","esp, N",s)
    return s
ra=[(i.address,i.mnemonic+" "+i.op_str) for i in cs.disasm(rb,base)]
ba=[(i.address,i.mnemonic+" "+i.op_str) for i in cs.disasm(built,base)]
rn=[norm(x[1]) for x in ra]; bn=[norm(x[1]) for x in ba]
sm=difflib.SequenceMatcher(None,rn,bn,autojunk=False); md=0; out=[]
for op,i1,i2,j1,j2 in sm.get_opcodes():
    if op=="equal": out.append(f"  == {i2-i1}"); continue
    for k in range(i1,i2): out.append(f"  - {ra[k][0]:08x} {ra[k][1]}"); md+=1
    for k in range(j1,j2): out.append(f"  + {ba[k][0]:08x} {ba[k][1]}"); md+=1
print("structural md", md)
if len(sys.argv)>3: print("\n".join(out))
