"""fwdis.py <exe> <va> [maxbytes]: disassemble a FableWin.exe / Ego_r.exe function at va (follows one incremental-link jmp), stops at ret. Read-only. Name its call targets with fwsym.py."""
import struct, sys
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
d=open(sys.argv[1],'rb').read(); va=int(sys.argv[2],16); mx=int(sys.argv[3]) if len(sys.argv)>3 else 600
pe=struct.unpack_from('<I',d,0x3c)[0]; ns=struct.unpack_from('<H',d,pe+6)[0]; op=struct.unpack_from('<H',d,pe+20)[0]
ib=struct.unpack_from('<I',d,pe+24+28)[0]
secs=[struct.unpack_from('<IIII',d,pe+24+op+40*i+8) for i in range(ns)]
def foff(rva):
    for vs,sva,rs,ro in secs:
        if sva<=rva<sva+max(vs,rs): return ro+rva-sva
md=Cs(CS_ARCH_X86,CS_MODE_32)
i=next(md.disasm(d[foff(va-ib):foff(va-ib)+8],va))
if i.mnemonic=='jmp': va=int(i.op_str,16)
for i in md.disasm(d[foff(va-ib):foff(va-ib)+mx],va):
    print('%08x %-20s %s %s'%(i.address,i.bytes.hex(),i.mnemonic,i.op_str))
    if i.mnemonic=='ret': break
