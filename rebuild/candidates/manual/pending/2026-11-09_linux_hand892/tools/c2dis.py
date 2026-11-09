import struct, sys
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
p="/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/wine/vc71rtm/bin/c2.dll"
b=open(p,"rb").read()
pe=struct.unpack_from("<I",b,0x3c)[0]; ns=struct.unpack_from("<H",b,pe+6)[0]; opt=struct.unpack_from("<H",b,pe+20)[0]
ib=struct.unpack_from("<I",b,pe+24+28)[0]
secs=[]
for i in range(ns):
    o=pe+24+opt+40*i; vs,sva,rs,ro=struct.unpack_from("<IIII",b,o+8); secs.append((sva,rs,ro))
def off(va):
    r=va-ib
    for sva,rs,ro in secs:
        if sva<=r<sva+rs: return ro+r-sva
s=int(sys.argv[1],16); n=int(sys.argv[2],16) if len(sys.argv)>2 else 0x80
md=Cs(CS_ARCH_X86,CS_MODE_32)
for ins in md.disasm(b[off(s):off(s)+n], s): print(hex(ins.address), ins.mnemonic, ins.op_str)
