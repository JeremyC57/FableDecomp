import mmap, struct, sys
f=open('/tmp/claude-0/egod_pdb/Ego_d.pdb','rb'); m=mmap.mmap(f.fileno(),0,access=mmap.ACCESS_READ)
PROC=(0x1110,0x110F)
def lines(off,cb):
    key=struct.pack('<IHHI',off,2,0,cb); i=m.find(key)
    if i<0: return None
    sublen=struct.unpack_from('<I',m,i-4)[0]; k=i+12; out=[]
    while k<i+sublen:
        fid,n,cbb=struct.unpack_from('<III',m,k)
        for j in range(n):
            o,li=struct.unpack_from('<II',m,k+12+j*8); out.append((o,li&0xffffff))
        k+=cbb
    return out
name=sys.argv[1]; key=name.encode()+b'\0'; pos=0
while True:
    i=m.find(key,pos)
    if i<0: print(name,'not found'); break
    pos=i+1; j=i-39
    ln,kd=struct.unpack_from('<HH',m,j)
    if kd not in PROC or not (i+len(key)<=j+2+ln<=i+len(key)+4): continue
    plen,dbgs,dbge,ti,poff,pseg=struct.unpack_from('<IIIIIH',m,j+16)
    L=lines(poff,plen)
    if L is None: continue
    print(name,'len',plen,'off',hex(poff))
    for k,(o,li) in enumerate(L):
        nx=L[k+1][0] if k+1<len(L) else plen
        print(f"{li:6d} {o:5x} {nx-o:5d}")
    break
