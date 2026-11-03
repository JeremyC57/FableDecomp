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
    return b''
for a in sys.argv[1:]:
    k,_,v=a.partition(':'); va=int(v,16)
    b=rd(va,256)
    if k=='w': 
        s=b.decode('utf-16le',errors='replace'); print(hex(va),'W',repr(s.split('\0')[0]))
    elif k=='s': print(hex(va),'S',repr(b.split(b'\0')[0]))
    elif k=='d': print(hex(va),'D',struct.unpack('<d',b[:8])[0])
    elif k=='f': print(hex(va),'F',struct.unpack('<f',b[:4])[0])
    elif k=='i': print(hex(va),'I',struct.unpack('<i',b[:4])[0])
    elif k=='x': print(hex(va),'X',b[:32].hex())
