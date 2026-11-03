# coff.py: minimal COFF reader -> (bytes, {offset: symbol}) for the largest function symbol containing a substring
import struct,sys
def load(path,leaf):
    d=open(path,'rb').read()
    nsec,_,symoff,nsym=struct.unpack_from('<HHIII',d,2)[0],0,struct.unpack_from('<I',d,8)[0],struct.unpack_from('<I',d,12)[0]
    optsz=struct.unpack_from('<H',d,16)[0]
    secs=[]
    for i in range(nsec):
        o=20+optsz+40*i
        name,vs,va,sz,ptr,rel,ln,nrel,nln,ch=struct.unpack_from('<8sIIIIIIHHI',d,o)
        secs.append((sz,ptr,rel,nrel))
    strtab=symoff+18*nsym
    def nm(o):
        if d[o:o+4]==b'\0\0\0\0':
            so=struct.unpack_from('<I',d,o+4)[0]; e=d.index(b'\0',strtab+so); return d[strtab+so:e].decode('latin1')
        return d[o:o+8].rstrip(b'\0').decode('latin1')
    syms={}; i=0; best=None
    while i<nsym:
        o=symoff+18*i
        val,sec,typ,cls,naux=struct.unpack_from('<IhHBB',d,o+8)
        n=nm(o); syms[i]=n
        if leaf in n and sec>0 and typ==0x20:
            sz=secs[sec-1][0]
            if best is None or sz>best[1]: best=(sec,sz,n)
        i+=1+naux
    sec,sz,n=best
    sz,ptr,rel,nrel=secs[sec-1]
    code=d[ptr:ptr+sz]; rels={}
    for k in range(nrel):
        va,si,ty=struct.unpack_from('<IIH',d,rel+10*k); rels[va]=syms.get(si,'?')
    return n,code,rels
if __name__=='__main__':
    n,code,rels=load(sys.argv[1],sys.argv[2]); print(n,len(code),len(rels))
