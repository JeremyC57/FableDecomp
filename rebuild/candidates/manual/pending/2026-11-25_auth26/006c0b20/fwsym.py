"""fwsym.py <va>...: resolve FableWin ILT thunk VAs to public names (read-only)."""
import struct, sys, re
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
d=open('/tmp/claude-0/egod_pdb/FableWin.exe','rb').read()
pe=struct.unpack_from('<I',d,0x3c)[0]; ns=struct.unpack_from('<H',d,pe+6)[0]; op=struct.unpack_from('<H',d,pe+20)[0]
secs=[struct.unpack_from('<IIII',d,pe+24+op+40*i+8) for i in range(ns)]
def foff(rva):
    for vs,sva,rs,ro in secs:
        if sva<=rva<sva+max(vs,rs): return ro+rva-sva
md=Cs(CS_ARCH_X86,CS_MODE_32)
want={}
for a in sys.argv[1:]:
    va=int(a,16); i=next(md.disasm(d[foff(va-0x400000):foff(va-0x400000)+8],va))
    t=int(i.op_str,16) if i.mnemonic=='jmp' else va
    want['0002:%d'%(t-0x400000-0x138F000)]=a
prev=None
for line in open('/tmp/claude-0/egod_pdb/FableWin.publics.txt'):
    m=re.search(r'addr = (\d+:\d+)',line)
    if m and m.group(1) in want: print(want[m.group(1)], prev.strip().split('`')[1])
    if 'S_PUB32' in line: prev=line
