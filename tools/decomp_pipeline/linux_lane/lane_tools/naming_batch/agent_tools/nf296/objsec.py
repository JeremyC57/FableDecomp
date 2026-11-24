# objsec.py head.obj new.obj [old=new ...]: compare every non-debug section (raw bytes + relocation list: offset, symbol, type) of two
# COFF objects after renaming the given old symbols; prints DIFF lines, ALL_EQ / bad count, and the number of equal .text sections.
import sys,struct
def load(p):
    b=open(p,'rb').read(); nsec=struct.unpack_from('<H',b,2)[0]; symoff,nsym=struct.unpack_from('<II',b,8); opt=struct.unpack_from('<H',b,16)[0]
    st=symoff+nsym*18
    def nm(e):
        if e[:4]==b'\0\0\0\0':
            o=struct.unpack_from('<I',e,4)[0]; return b[st+o:b.index(b'\0',st+o)].decode('latin1')
        return e[:8].rstrip(b'\0').decode('latin1')
    syms={}; i=0
    while i<nsym:
        e=b[symoff+i*18:symoff+i*18+18]; syms[i]=nm(e); i+=1+e[17]
    secs=[]
    for k in range(nsec):
        h=b[20+opt+40*k:20+opt+40*(k+1)]; name=h[:8].rstrip(b'\0').decode('latin1')
        size,ptr,prel,_,nrel=struct.unpack_from('<IIIIH',h,16)
        if name.startswith('.debug'): continue
        raw=b[ptr:ptr+size] if ptr else b''
        rel=[(struct.unpack_from('<I',b,prel+10*j)[0],syms.get(struct.unpack_from('<I',b,prel+10*j+4)[0],'?'),struct.unpack_from('<H',b,prel+10*j+8)[0]) for j in range(nrel)]
        secs.append((name,raw,rel))
    return secs
h=load(sys.argv[1]); n=load(sys.argv[2]); mp=dict(x.split('=',1) for x in sys.argv[3:])
print('sections head',len(h),'new',len(n)); bad=0; text=0
if len(h)!=len(n): bad+=1; print('DIFF section count')
for i,(a,b) in enumerate(zip(h,n)):
    ra=[(o,mp.get(s,s),t) for o,s,t in a[2]]; rb=b[2]
    if a[1]==b[1] and ra==rb and a[0]==b[0]:
        if a[0]=='.text': text+=1
        continue
    bad+=1; print('DIFF',i,a[0],b[0],'bytes',a[1]==b[1],'relocs',ra==rb,len(a[1]),len(b[1]))
    for x,y in zip(ra,rb):
        if x!=y: print('   ',x,'|',y)
print('ALL_EQ' if not bad else 'bad %d'%bad, 'text_sections_equal=%d'%text)
