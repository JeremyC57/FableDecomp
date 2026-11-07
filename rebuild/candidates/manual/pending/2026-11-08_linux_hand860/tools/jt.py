# jt.py <addr> <tag> <leaf>: list jne/je targets after `test al,al` in our build, with target's first instruction
import sys,os
sys.path.insert(0,"/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
S=os.environ['S']; a,tag,leaf=sys.argv[1:4]
b,sec,sym=V.obj_text(f"{S}/w22/agent860/w/{a}_{tag}/{a}.obj",leaf)[:3]
base=int(a,16); ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(b,base)); at={i.address:i for i in ins}
for k,i in enumerate(ins):
    if i.mnemonic in('jne','je') and k and ins[k-1].op_str=='al, al' and k>1 and ins[k-2].mnemonic=='call':
        t=int(i.op_str,16)
        if t>i.address: print(f"{i.address:08x} {i.mnemonic} {t:08x}: {at[t].mnemonic} {at[t].op_str}" if t in at else f"{i.address:08x} -> {t:08x}")
