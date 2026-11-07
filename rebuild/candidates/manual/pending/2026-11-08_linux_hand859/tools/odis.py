# odis.py <addr> <tag> <leaf> [from] [to]: disassemble our build (reloc fields shown as-is)
import sys,os
sys.path.insert(0,"/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
S=os.environ['S']; a,tag,leaf=sys.argv[1:4]
b=V.obj_text(f"{S}/wine/q859/{a}_{tag}/{a}.obj",leaf)[0]
base=int(a,16); lo=int(sys.argv[4],16) if len(sys.argv)>4 else base; hi=int(sys.argv[5],16) if len(sys.argv)>5 else base+len(b)
for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(b,base):
    if lo<=i.address<hi: print(f"{i.address:08x} {i.bytes.hex():20} {i.mnemonic} {i.op_str}")
