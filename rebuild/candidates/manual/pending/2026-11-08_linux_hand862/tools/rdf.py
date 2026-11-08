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
va=int(sys.argv[1],16); n=int(sys.argv[2])
b=rd(va,4*n)
for i in range(n):
    w=struct.unpack_from("<I",b,4*i)[0]; f=struct.unpack_from("<f",b,4*i)[0]
    print(f"{va+4*i:08x} {w:08x} {f}")
