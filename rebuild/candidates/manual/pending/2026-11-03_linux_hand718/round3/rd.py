import os,sys,struct
S=os.environ["S"]
exe=open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
pe=struct.unpack_from("<I",exe,0x3c)[0]
nsec=struct.unpack_from("<H",exe,pe+6)[0]; opt=struct.unpack_from("<H",exe,pe+20)[0]
secs=[]
for i in range(nsec):
    o=pe+24+opt+40*i; vs,va,rs,ro=struct.unpack_from("<IIII",exe,o+8); secs.append((va,rs,ro))
def rd(va,n):
    for sva,rs,ro in secs:
        if sva<=va-0x400000<sva+rs: off=va-0x400000-sva+ro; return exe[off:off+n]
for a in sys.argv[1:]:
    k,_,t=a.partition(':'); va=int(k,16)
    if t=='s': b=rd(va,200); print(hex(va), b.split(b'\0')[0])
    elif t=='w': b=rd(va,400); print(hex(va), b.decode('utf-16le','replace').split('\0')[0])
    elif t=='d': print(hex(va), struct.unpack('<d',rd(va,8))[0])
    elif t=='i': print(hex(va), hex(struct.unpack('<I',rd(va,4))[0]))
    else: print(hex(va), struct.unpack('<f',rd(va,4))[0])
