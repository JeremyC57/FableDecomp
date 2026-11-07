# mt.py <src.cpp> <leaf> [flags...]: compile with overlay, print disasm of leaf
import sys,os
sys.path.insert(0,"/home/user/FableDecomp/tools/decomp_pipeline")
S=os.environ['S']; A=S+'/w22/agent860'; os.environ["FABLE_HDR_OVERLAY"]=A+"/hdr"
import verify_and_land as V
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
src,leaf=sys.argv[1:3]; fl=sys.argv[3:]
w=Path(A)/'w'/'mt'; w.mkdir(parents=True,exist_ok=True)
sp=w/'mt.cpp'; sp.write_text(V.vc71(open(src).read()))
e=V.env()
cp=V.cl(["/nologo","/c"]+V.BASE_FLAGS+fl+[f"/Fo{w/'mt.obj'}",str(sp)],e)
if cp.returncode: print(cp.stdout[-3000:]); sys.exit(1)
b=V.obj_text(str(w/'mt.obj'),leaf)[0]
for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(b,0): print(f"{i.address:04x} {i.mnemonic} {i.op_str}")
