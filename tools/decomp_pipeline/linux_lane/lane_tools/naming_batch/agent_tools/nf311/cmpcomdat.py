# cmpcomdat.py <objA> <objB>: for every function COMDAT symbol defined in both objects, compare its section bytes and relocation list
# (offset, symbol, type) -> one line per shared symbol: bytes_equal / relocs_equal.
import sys,struct
def load(p):
    b=open(p,'rb').read(); nsec=struct.unpack_from('<H',b,2)[0]; symoff,nsym=struct.unpack_from('<II',b,8); opt=struct.unpack_from('<H',b,16)[0]
    st=symoff+nsym*18
    def nm(e):
        if e[:4]==b'\0\0\0\0':
            o=struct.unpack_from('<I',e,4)[0]; return b[st+o:b.index(b'\0',st+o)].decode('latin1')
        return e[:8].rstrip(b'\0').decode('latin1')
    syms={}; defs={}; i=0
    while i<nsym:
        e=b[symoff+i*18:symoff+i*18+18]; n=nm(e); val,sec,typ,scl=struct.unpack_from('<IhHB',e,8); syms[i]=n
        if scl==2 and sec>0 and typ==0x20: defs[n]=sec
        i+=1+e[17]
    out={}
    for n,sec in defs.items():
        h=b[20+opt+40*(sec-1):20+opt+40*sec]; size,ptr,prel,_,nrel=struct.unpack_from('<IIIIH',h,16)
        rel=[(struct.unpack_from('<I',b,prel+10*j)[0],syms.get(struct.unpack_from('<I',b,prel+10*j+4)[0],'?'),struct.unpack_from('<H',b,prel+10*j+8)[0]) for j in range(nrel)]
        out[n]=(b[ptr:ptr+size],rel)
    return out
a=load(sys.argv[1]); c=load(sys.argv[2])
for n in sorted(set(a)&set(c)):
    print('%s bytes_equal=%s relocs_equal=%s sizes=%d/%d'%(n[:110],a[n][0]==c[n][0],a[n][1]==c[n][1],len(a[n][0]),len(c[n][0])))
