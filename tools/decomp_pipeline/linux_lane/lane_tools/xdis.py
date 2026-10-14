import os,sys,struct
from capstone import *
S=os.environ["S"]
exe=open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
pe=struct.unpack_from("<I",exe,0x3c)[0]
nsec=struct.unpack_from("<H",exe,pe+6)[0]; opt=struct.unpack_from("<H",exe,pe+20)[0]
secs=[]
for i in range(nsec):
    o=pe+24+opt+40*i; vs,va,rs,ro=struct.unpack_from("<IIII",exe,o+8); secs.append((va,rs,ro))
def rd(va,n):
    for sva,rs,ro in secs:
        if sva<=va-0x400000<sva+rs: off=va-0x400000-sva+ro; return exe[off:off+n]
md=Cs(CS_ARCH_X86,CS_MODE_32)
va=int(sys.argv[1],16); n=int(sys.argv[2],16)
for i in md.disasm(rd(va,n),va):
    print(f"{i.address:08x} {i.bytes.hex():20} {i.mnemonic} {i.op_str}")
