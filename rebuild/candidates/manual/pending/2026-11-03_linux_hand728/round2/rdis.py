import struct,sys
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
exe=open("/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
pe=struct.unpack_from("<I",exe,0x3c)[0]; so=struct.unpack_from("<H",exe,pe+20)[0]
o=pe+24+so; va=struct.unpack_from("<I",exe,o+12)[0]; raw=struct.unpack_from("<I",exe,o+20)[0]
a=int(sys.argv[1],16); n=int(sys.argv[2],16)
off=a-0x400000-va+raw
md=Cs(CS_ARCH_X86,CS_MODE_32)
for i in md.disasm(exe[off:off+n],a): print("%08x %-20s %s %s"%(i.address,i.bytes.hex(),i.mnemonic,i.op_str))
