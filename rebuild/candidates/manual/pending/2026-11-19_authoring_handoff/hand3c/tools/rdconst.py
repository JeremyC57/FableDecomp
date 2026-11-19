#!/usr/bin/env python3
# rdconst.py <exe> <hexva> [f|d] ... : print float (f) or double (d) constants at VAs of a PE32 (section-mapped read).
import sys, struct
def load(p):
    b=open(p,'rb').read(); pe=struct.unpack_from('<I',b,0x3c)[0]
    nsec=struct.unpack_from('<H',b,pe+6)[0]; opt=struct.unpack_from('<H',b,pe+20)[0]
    base=struct.unpack_from('<I',b,pe+24+28)[0]; secs=[]; o=pe+24+opt
    for i in range(nsec):
        vs,va,rs,ro=struct.unpack_from('<IIII',b,o+8); secs.append((va,max(vs,rs),ro)); o+=40
    return b,base,secs
b,base,secs=load(sys.argv[1])
def rd(va,n):
    rva=va-base
    for sva,sz,ro in secs:
        if sva<=rva<sva+sz: return b[ro+rva-sva:ro+rva-sva+n]
args=sys.argv[2:]
for a in args:
    kind='f'
    if ':' in a: a,kind=a.split(':')
    va=int(a,16)
    print(hex(va), struct.unpack('<'+kind, rd(va,8 if kind=='d' else 4))[0])
