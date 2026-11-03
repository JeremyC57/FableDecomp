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
cat={}
for l in open("rebuild/oracles/auto-re-candidates.tsv"):
    p=l.split("\t")
    if len(p)>1: cat[p[0].lower()]=p[1]
va=int(sys.argv[1],16); n=int(sys.argv[2]) if len(sys.argv)>2 else 12
d=rd(va-4,4*(n+1))
print("rtti col", hex(struct.unpack_from("<I",d,0)[0]))
for i in range(n):
    x=struct.unpack_from("<I",d,4*(i+1))[0]
    print(i, f"{x:08x}", cat.get(f"{x:08x}",""))
