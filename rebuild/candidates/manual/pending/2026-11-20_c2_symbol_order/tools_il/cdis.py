#!/usr/bin/env python3
"""cdis.py <hexva> [hexlen]        : disassemble c2.dll (13.10.3077) at VA
   cdis.py fn <hexva> [maxlen]     : disassemble the whole function containing VA (function starts = call targets)
   cdis.py who <hexva>             : function start containing VA
   cdis.py xref <hexva>            : call sites targeting VA
   cdis.py str <hexva>             : string at VA"""
import struct, sys, os, pickle
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
p=os.environ['FABLE_VC71']+'/bin/c2.dll'
b=open(p,"rb").read()
pe=struct.unpack_from("<I",b,0x3c)[0]; ns=struct.unpack_from("<H",b,pe+6)[0]; opt=struct.unpack_from("<H",b,pe+20)[0]
ib=struct.unpack_from("<I",b,pe+24+28)[0]
secs=[]
for i in range(ns):
    o=pe+24+opt+40*i; vs,sva,rs,ro=struct.unpack_from("<IIII",b,o+8); secs.append((sva,max(vs,rs),ro,b[o:o+8].rstrip(b'\0').decode()))
def off(va):
    r=va-ib
    for sva,rs,ro,_ in secs:
        if sva<=r<sva+rs: return ro+r-sva
TS=ib+secs[0][0]; TE=TS+secs[0][1]
cache=os.path.dirname(os.path.abspath(__file__))+'/c2calls.pkl'
def index():
    if os.path.exists(cache): return pickle.load(open(cache,'rb'))
    t=b[off(TS):off(TS)+secs[0][1]]; calls={}
    i=0
    while True:
        i=t.find(b'\xe8',i)
        if i<0 or i+5>len(t): break
        tgt=TS+i+5+struct.unpack_from('<i',t,i+1)[0]
        if TS<=tgt<TE: calls.setdefault(tgt,[]).append(TS+i)
        i+=1
    # keep targets that look like function starts (called >=1) ; filter false positives by requiring target preceded by ret/int3/nop/jmp or >=2 callers
    good={}
    for tgt,srcs in calls.items():
        pb=t[tgt-TS-1] if tgt>TS else 0xcc
        if len(srcs)>=2 or pb in (0xc3,0xcc,0x90) or t[tgt-TS-3:tgt-TS-2]==b'\xc2' or t[tgt-TS-5:tgt-TS-4]==b'\xe9':
            good[tgt]=srcs
    pickle.dump(good,open(cache,'wb')); return good
md=Cs(CS_ARCH_X86,CS_MODE_32)
def dis(s,n):
    for ins in md.disasm(b[off(s):off(s)+n], s): print(hex(ins.address), ins.mnemonic, ins.op_str)
def who(va):
    g=sorted(index()); import bisect
    i=bisect.bisect_right(g,va)-1; return g[i], (g[i+1] if i+1<len(g) else TE)
if __name__=='__main__':
    a=sys.argv
    if a[1]=='fn':
        s,e=who(int(a[2],16)); n=min(e-s, int(a[3],16) if len(a)>3 else 0x600); print('; function',hex(s),'..',hex(e)); dis(s,n)
    elif a[1]=='who':
        for x in a[2:]:
            s,e=who(int(x,16)); print(x,'in',hex(s),'..',hex(e),'callers',len(index().get(s,[])))
    elif a[1]=='xref':
        print(' '.join(hex(x) for x in index().get(int(a[2],16),[])))
    elif a[1]=='str':
        o=off(int(a[2],16)); print(b[o:o+120].split(b'\0')[0])
    else: dis(int(a[1],16), int(a[2],16) if len(a)>2 else 0x80)
