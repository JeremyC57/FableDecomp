import sys
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/bounds')
from extent import Exe
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
E=Exe(); md=Cs(CS_ARCH_X86,CS_MODE_32)
va=int(sys.argv[1],16); n=int(sys.argv[2]) if len(sys.argv)>2 else 400
for i in md.disasm(E.read(va,n),va): print(hex(i.address),i.mnemonic,i.op_str)
