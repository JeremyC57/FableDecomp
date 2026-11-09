import sys
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
obj=sys.argv[1]; n=int(sys.argv[2]) if len(sys.argv)>2 else 60
leaf='?Action@CManager@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z'
built,sec,sym=V.obj_text(obj,leaf)
md=Cs(CS_ARCH_X86,CS_MODE_32)
for k,i in enumerate(md.disasm(built,0)):
    if k>=n: break
    print(f'{i.address:05x} {i.mnemonic} {i.op_str}')
