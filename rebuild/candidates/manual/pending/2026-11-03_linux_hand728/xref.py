import struct,os,sys
S=os.environ["S"]
exe=open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
pe=struct.unpack_from("<I",exe,0x3c)[0]
nsec=struct.unpack_from("<H",exe,pe+6)[0]; opt=struct.unpack_from("<H",exe,pe+20)[0]
secs=[]
for i in range(nsec):
    o=pe+24+opt+40*i; vs,va,rs,ro=struct.unpack_from("<IIII",exe,o+8); secs.append((va,rs,ro))
def off2va(off):
    for va,rs,ro in secs:
        if ro<=off<ro+rs: return off-ro+va+0x400000
for a in sys.argv[1:]:
    t=int(a,16); p=struct.pack("<I",t); i=exe.find(p); res=[]
    while i>=0: res.append(hex(off2va(i))); i=exe.find(p,i+1)
    # rel32 call/jmp
    calls=[]
    tsec=secs[0]
    print(a,'abs:',res[:40])
