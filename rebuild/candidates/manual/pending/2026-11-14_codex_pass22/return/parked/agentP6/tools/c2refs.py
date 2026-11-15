"""c2refs.py <c2.dll> <hexaddr> [window]: every instruction referencing the absolute address, plus disasm context."""
import sys,struct
from capstone import *
b=open(sys.argv[1],'rb').read(); tgt=int(sys.argv[2],16); W=int(sys.argv[3]) if len(sys.argv)>3 else 0
pe=struct.unpack_from('<I',b,0x3c)[0]; nsec=struct.unpack_from('<H',b,pe+6)[0]; opt=struct.unpack_from('<H',b,pe+20)[0]
base=0x10700000; secs=[]
for i in range(nsec):
    o=pe+24+opt+40*i; vs,va,rs,ro=struct.unpack_from('<IIII',b,o+8); secs.append((va,rs,ro))
def rd(a,n):
    for va,rs,ro in secs:
        if va<=a-base<va+rs: off=a-base-va+ro; return b[off:off+n]
def f2va(off):
    for va,rs,ro in secs:
        if ro<=off<ro+rs: return off-ro+va+base
md=Cs(CS_ARCH_X86,CS_MODE_32); pat=struct.pack('<I',tgt); i=b.find(pat); seen=set()
while i>=0:
    va=f2va(i)
    for back in range(1,7):
        ins=next(md.disasm(rd(va-back,15),va-back),None)
        if ins and ins.address+ins.size>=va+4 and ('0x%x'%tgt) in ins.op_str and ins.address not in seen:
            seen.add(ins.address); print(hex(ins.address),ins.mnemonic,ins.op_str)
            if W:
                for j in md.disasm(rd(ins.address-W,W+24),ins.address-W): print('     ',hex(j.address),j.mnemonic,j.op_str)
            break
    i=b.find(pat,i+1)
