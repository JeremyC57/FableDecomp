#!/usr/bin/env python3
# egod_proc.py <pdb> <qualified name> : S_GPROC32 offset/segment/length + blocks (offset,len) for egod_lines.py
import sys,struct,mmap
f=open(sys.argv[1],'rb'); m=mmap.mmap(f.fileno(),0,access=mmap.ACCESS_READ)
key=sys.argv[2].encode()+b'\0'; pos=0
while True:
    i=m.find(key,pos)
    if i<0: break
    pos=i+1; j=i-39
    ln,kd=struct.unpack_from('<HH',m,j)
    if kd not in (0x1110,0x110F) or not (i+len(key)<=j+2+ln<=i+len(key)+4): continue
    plen=struct.unpack_from('<I',m,j+16)[0]; off,seg=struct.unpack_from('<IH',m,j+32)
    print('proc off=%d seg=%d len=%d'%(off,seg,plen))
    k=j+2+ln; d=0
    for _ in range(2000):
        l2,k2=struct.unpack_from('<HH',m,k)
        if k2==6:
            if d==0: break
            d-=1; print('  '*d+'}')
        elif k2==0x1103:
            bl,bo,bs=struct.unpack_from('<IIH',m,k+12); print('  '*d+'{ +%04x len %d'%(bo-off,bl)); d+=1
        elif k2 in (0x110B,0x1111):
            nm=m[k+12 if k2==0x110B else k+14:m.find(b'\0',k+12 if k2==0x110B else k+14)].decode('latin1'); print('  '*d+'  '+nm)
        k+=2+l2
    break
