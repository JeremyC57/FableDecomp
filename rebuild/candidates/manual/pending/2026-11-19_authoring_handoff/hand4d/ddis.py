# ddis.py <exe: fw|ed|er> <hexva> <decimal len> : disassemble the debug builds (image base from header)
import sys,struct
from capstone import *
P={"fw":"/tmp/claude-0/egod_pdb/FableWin.exe","ed":"/tmp/claude-0/egod_pdb/Ego_d.exe","er":"/tmp/claude-0/egod_pdb/ego_r.exe"}
exe=open(P[sys.argv[1]],"rb").read()
pe=struct.unpack_from("<I",exe,0x3c)[0]
nsec=struct.unpack_from("<H",exe,pe+6)[0]; opt=struct.unpack_from("<H",exe,pe+20)[0]
base=struct.unpack_from("<I",exe,pe+24+28)[0]
secs=[]
for i in range(nsec):
    o=pe+24+opt+40*i; vs,va,rs,ro=struct.unpack_from("<IIII",exe,o+8); secs.append((va,rs,ro))
def rd(va,n):
    for sva,rs,ro in secs:
        if sva<=va-base<sva+rs: off=va-base-sva+ro; return exe[off:off+n]
md=Cs(CS_ARCH_X86,CS_MODE_32)
va=int(sys.argv[2],16); n=int(sys.argv[3])
for i in md.disasm(rd(va,n),va):
    print(f"{i.address:08x} {i.bytes.hex():20} {i.mnemonic} {i.op_str}")
