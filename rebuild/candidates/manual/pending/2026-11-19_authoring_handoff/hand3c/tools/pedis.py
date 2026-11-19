#!/usr/bin/env python3
# pedis.py <exe> <hexva> <hexlen>
import sys, struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
d=open(sys.argv[1],"rb").read(); va=int(sys.argv[2],16); n=int(sys.argv[3],16)
pe=struct.unpack_from("<I",d,0x3c)[0]; ns=struct.unpack_from("<H",d,pe+6)[0]; opt=struct.unpack_from("<H",d,pe+20)[0]
base=struct.unpack_from("<I",d,pe+24+28)[0]
for i in range(ns):
    o=pe+24+opt+40*i; vs,v,rs,r=struct.unpack_from("<IIII",d,o+8)
    if v<=va-base<v+max(vs,rs):
        off=r+va-base-v; break
md=Cs(CS_ARCH_X86,CS_MODE_32)
for ins in md.disasm(d[off:off+n],va): print(f"{ins.address:08x} {ins.bytes.hex():20} {ins.mnemonic} {ins.op_str}")
