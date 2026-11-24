"""egl.py <qualified name>: Ego_d line table of a function (finds S_GPROC32/S_LPROC32 by name, then the C13 0xF2 lines block)."""
import mmap, struct, sys, re
f=open('/tmp/claude-0/egod_pdb/Ego_d.pdb','rb'); m=mmap.mmap(f.fileno(),0,access=mmap.ACCESS_READ)
name=sys.argv[1].encode()+b'\0'
seen=set(); pos=0
while True:
    k=m.find(name,pos)
    if k<0: break
    pos=k+1
    j=k-39
    if j<0: continue
    ln,kd=struct.unpack_from('<HH',m,j)
    if kd not in (0x1110,0x110F): continue
    plen,=struct.unpack_from("<I",m,j+16); off,seg=struct.unpack_from("<IH",m,j+32)
    if (off,plen) in seen: continue
    seen.add((off,plen))
    pat=struct.pack('<IHHI',off,seg,0,plen)
    q=0
    while True:
        i=m.find(pat,q)
        if i<0: break
        q=i+1
        if struct.unpack_from('<I',m,i-8)[0]!=0xF2: continue
        sublen=struct.unpack_from('<I',m,i-4)[0]; kk=i+12; end=i+sublen
        print('== %s off %x len %d'%(sys.argv[1],off,plen))
        while kk<end:
            fid,n,cbb=struct.unpack_from('<III',m,kk)
            prev=None
            for t in range(n):
                o,li=struct.unpack_from('<II',m,kk+12+t*8); l=li&0xffffff
                if prev: print('  line %-6d %4d B'%(prev[1],o-prev[0]))
                prev=(o,l)
            if prev: print('  line %-6d %4d B'%(prev[1],plen-prev[0]))
            kk+=cbb
        break
