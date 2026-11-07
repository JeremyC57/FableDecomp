# xdis.py <va_hex> <len_hex>: disassemble arbitrary retail bytes from Fable.exe
import sys,os,struct
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
S=os.environ['S']; b=open(S+'/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
pe=struct.unpack_from('<I',b,0x3c)[0]; n=struct.unpack_from('<H',b,pe+6)[0]; opt=struct.unpack_from('<H',b,pe+20)[0]
secs=[struct.unpack_from('<8sIIII',b,pe+24+opt+40*i) for i in range(n)]
def off(va):
    rva=va-0x400000
    for nm,vs,va_,rs,ro in secs:
        if va_<=rva<va_+max(vs,rs): return rva-va_+ro
va=int(sys.argv[1],16); ln=int(sys.argv[2],16); o=off(va)
for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(b[o:o+ln],va): print(f"{i.address:08x} {i.mnemonic} {i.op_str}")
