# rexe.py <from> <to>: disassemble retail Fable.exe range
import sys,os,struct
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
S=os.environ['S']; b=open(S+'/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
pe=struct.unpack_from('<I',b,0x3c)[0]; ns=struct.unpack_from('<H',b,pe+6)[0]; opt=struct.unpack_from('<H',b,pe+20)[0]
lo,hi=int(sys.argv[1],16),int(sys.argv[2],16)
for i in range(ns):
    o=pe+24+opt+40*i; vs,va,rs,ro=struct.unpack_from('<IIII',b,o+8)
    if 0x400000+va<=lo<0x400000+va+rs:
        off=lo-0x400000-va+ro; data=b[off:off+hi-lo]
        for ins in Cs(CS_ARCH_X86,CS_MODE_32).disasm(data,lo): print(f"{ins.address:08x} {ins.bytes.hex():20} {ins.mnemonic} {ins.op_str}")
