import sys
sys.path.insert(0,'/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/chunk729')
from coff import load
from capstone import *
n,code,r=load(sys.argv[1],sys.argv[2]); seq=[]
for ins in Cs(CS_ARCH_X86,CS_MODE_32).disasm(code,0):
    if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
        s=r.get(ins.address+1,'')
        if 'Reset' in s: seq.append('R')
        elif s.startswith('??2'): seq.append('i')
print(len(code),''.join(seq))
