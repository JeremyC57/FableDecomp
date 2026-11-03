# xref.py <target-hex> [lo hi]: find rel32 call/jmp sites to target in .text
import os,struct,sys
S=os.environ["S"]
exe=open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
pe=struct.unpack_from("<I",exe,0x3c)[0]
nsec=struct.unpack_from("<H",exe,pe+6)[0]; opt=struct.unpack_from("<H",exe,pe+20)[0]
t=int(sys.argv[1],16)
for i in range(nsec):
    o=pe+24+opt+40*i; vs,va,rs,ro=struct.unpack_from("<IIII",exe,o+8)
    if exe[o:o+5]!=b'.text': continue
    d=exe[ro:ro+rs]
    for k in range(len(d)-5):
        if d[k] in (0xe8,0xe9):
            r=struct.unpack_from("<i",d,k+1)[0]; src=0x400000+va+k
            if src+5+r==t: print(hex(src), 'call' if d[k]==0xe8 else 'jmp')
