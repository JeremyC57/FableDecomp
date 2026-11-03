# relocs.py <obj> <leafsym> <retail_va_hex> <oracle.tsv>: relocations of the leaf's COMDAT with the retail target at each offset
import struct,sys
obj,leaf,va,orc=sys.argv[1:5]; va=int(va,16)
rows={l.split('\t')[0]:l.rstrip('\n').split('\t') for l in open(orc)}
byt=bytes.fromhex(rows['%08x'%va][-1])
d=open(obj,'rb').read()
mach,nsec,ts,symptr,nsym,opt,ch=struct.unpack_from('<HHIIIHH',d,0)
secs=[struct.unpack_from('<8sIIIIIIHHI',d,20+opt+40*i) for i in range(nsec)]
strtab=symptr+18*nsym
def name(i):
    raw=d[symptr+18*i:symptr+18*i+8]
    if raw[:4]==b'\0\0\0\0':
        o=struct.unpack_from('<I',raw,4)[0]; e=d.index(b'\0',strtab+o); return d[strtab+o:e].decode()
    return raw.rstrip(b'\0').decode()
sec=None;i=0
while i<nsym:
    n=name(i); val,secn,typ,cls,aux=struct.unpack_from('<IhHBB',d,symptr+18*i+8)
    if n==leaf: sec=secn
    i+=1+aux
s=secs[sec-1]; rptr,nrel=s[5],s[7]
out=[]
for k in range(nrel):
    off,si,ty=struct.unpack_from('<IIH',d,rptr+10*k)
    v=int.from_bytes(byt[off:off+4],'little')
    tgt=(va+off+4+v)&0xffffffff if ty==0x14 else v
    out.append((tgt,off,name(si)))
for tgt,off,n in sorted(set((t,0,n) for t,o,n in out)): print('%08x\t%s'%(tgt,n))
