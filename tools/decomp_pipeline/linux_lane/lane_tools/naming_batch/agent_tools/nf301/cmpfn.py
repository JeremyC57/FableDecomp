# cmpfn.py head.obj new.obj [old=new ...]: compare COMDAT / section contents BY the symbol defined at offset 0 of each section
# (after renaming), bytes + relocation lists; prints SAME / DIFF / ONLY_HEAD / ONLY_NEW per symbol
import sys,struct
def load(p):
    b=open(p,'rb').read(); nsec=struct.unpack_from('<H',b,2)[0]; symoff,nsym=struct.unpack_from('<II',b,8); opt=struct.unpack_from('<H',b,16)[0]
    st=symoff+nsym*18
    def nm(e):
        if e[:4]==b'\0\0\0\0':
            o=struct.unpack_from('<I',e,4)[0]; return b[st+o:b.index(b'\0',st+o)].decode('latin1')
        return e[:8].rstrip(b'\0').decode('latin1')
    syms={}; owner={}; i=0
    while i<nsym:
        e=b[symoff+i*18:symoff+i*18+18]; n=nm(e); syms[i]=n
        val,sec=struct.unpack_from('<Ih',e,8); scl=e[16]
        if scl==2 and sec>0 and val==0 and sec not in owner: owner[sec]=n
        i+=1+e[17]
    out={}
    for k in range(nsec):
        h=b[20+opt+40*k:20+opt+40*(k+1)]; name=h[:8].rstrip(b'\0').decode('latin1')
        if name.startswith('.debug') or name=='.drectve': continue
        size,ptr,prel,_,nrel=struct.unpack_from('<IIIIH',h,16)
        raw=b[ptr:ptr+size] if ptr else b''
        rel=[(struct.unpack_from('<I',b,prel+10*j)[0],syms.get(struct.unpack_from('<I',b,prel+10*j+4)[0],'?')) for j in range(nrel)]
        out.setdefault(owner.get(k+1,name+'#%d'%k) if (k+1) in owner else name,[]).append((raw,rel))
    return out
h=load(sys.argv[1]); n=load(sys.argv[2]); mp=dict(x.split('=',1) for x in sys.argv[3:])
h={mp.get(k,k):[(r,[(o,mp.get(s,s)) for o,s in rl]) for r,rl in v] for k,v in h.items()}
for k in sorted(set(h)|set(n)):
    if k not in n: print('ONLY_HEAD',k); continue
    if k not in h: print('ONLY_NEW',k); continue
    print('SAME' if h[k]==n[k] else 'DIFF',k)
