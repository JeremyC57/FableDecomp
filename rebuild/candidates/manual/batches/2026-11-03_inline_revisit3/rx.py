#!/usr/bin/env python3
# rx.py <hexaddr> [n] | rx.py vt <vtable> <slots> : disassemble retail Fable.exe at an address (until ret) / dump vtable
import struct,sys
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
p='${S}/dl/game/Fable The Lost Chapters/Fable.exe'
d=open(p,'rb').read()
pe=struct.unpack('<I',d[0x3c:0x40])[0]; ns=struct.unpack('<H',d[pe+6:pe+8])[0]; oh=struct.unpack('<H',d[pe+20:pe+22])[0]
secs=[]
for i in range(ns):
    o=pe+24+oh+40*i; va,sz,raw=struct.unpack('<III',d[o+12:o+24]); secs.append((va,sz,raw))
def rd(a,n):
    rva=a-0x400000
    for va,sz,raw in secs:
        if va<=rva<va+max(sz,1): return d[raw+rva-va:raw+rva-va+n]
if sys.argv[1]=='vt':
    v=int(sys.argv[2],16)
    for k in range(int(sys.argv[3])): print(k, hex(struct.unpack('<I',rd(v+4*k,4))[0]))
    sys.exit()
a=int(sys.argv[1],16); n=int(sys.argv[2]) if len(sys.argv)>2 else 400
md=Cs(CS_ARCH_X86,CS_MODE_32)
for i in md.disasm(rd(a,n),a):
    print(f"{i.address:08x}: {i.mnemonic} {i.op_str}")
    if i.mnemonic in('ret',) and len(sys.argv)<=2: break
