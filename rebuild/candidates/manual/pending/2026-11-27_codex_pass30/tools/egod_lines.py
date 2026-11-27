#!/usr/bin/env python3
"""egod_lines.py <Ego_d.pdb> <sect> <off> <len> : find raw C13 DEBUG_S_LINES blocks for (off, sect) and print per-line byte counts."""
import sys, struct, mmap
f=open(sys.argv[1],'rb'); m=mmap.mmap(f.fileno(),0,access=mmap.ACCESS_READ)
sect=int(sys.argv[2]); off=int(sys.argv[3]); ln=int(sys.argv[4])
pat=struct.pack('<IH',off,sect)
k=0; seen=set()
while True:
    k=m.find(pat,k)
    if k<0: break
    # header: offCon u32, segCon u16, flags u16, cbCon u32, then file blocks {fileid u32, nLines u32, cb u32, lines...}
    flags,cb=struct.unpack_from('<HI',m,k+6)
    if cb==ln and flags in (0,1):
        fid,n,cbb=struct.unpack_from('<III',m,k+12)
        if 0<n<500:
            ent=[struct.unpack_from('<II',m,k+24+8*i) for i in range(n)]
            key=tuple(ent)
            if key not in seen:
                seen.add(key)
                print('# at file offset',k,'file id',fid,'nLines',n)
                for i,(o,l) in enumerate(ent):
                    nxt=ent[i+1][0] if i+1<n else ln
                    print('+%04x\t%d\t%d'%(o,l&0x7fffffff,nxt-o))
    k+=1
