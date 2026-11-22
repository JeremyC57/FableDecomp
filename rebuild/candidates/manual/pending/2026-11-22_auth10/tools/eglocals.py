import mmap, struct, sys
f=open('/tmp/claude-0/egod_pdb/Ego_d.pdb','rb'); m=mmap.mmap(f.fileno(),0,access=mmap.ACCESS_READ)
name=sys.argv[1]; key=name.encode()+b'\0'; pos=0
while True:
    i=m.find(key,pos)
    if i<0: print('not found'); break
    pos=i+1; j=i-39
    ln,kd=struct.unpack_from('<HH',m,j)
    if kd not in (0x1110,0x110F) or not (i+len(key)<=j+2+ln<=i+len(key)+4): continue
    k=j+2+ln; depth=0
    while True:
        rl,rk=struct.unpack_from('<HH',m,k)
        if rk==0x110B:
            off,ti=struct.unpack_from('<iI',m,k+4); nm=m[k+12:m.find(b'\0',k+12)].decode('latin1'); print('  '*depth+f"BP {off:5d} t{ti:x} {nm}")
        elif rk==0x1111:
            off,ti,rg=struct.unpack_from('<iIH',m,k+4); nm=m[k+14:m.find(b'\0',k+14)].decode('latin1'); print('  '*depth+f"RR {off:5d} r{rg} t{ti:x} {nm}")
        elif rk==0x1103:
            par,end,blen,boff,bseg=struct.unpack_from('<IIIIH',m,k+4); print('  '*depth+f"BLOCK off {boff:x} len {blen}"); depth+=1
        elif rk==0x0006:
            if depth==0: break
            depth-=1
        elif rk in (0x1110,0x110F): print('nested proc?'); break
        k+=2+rl
    break
