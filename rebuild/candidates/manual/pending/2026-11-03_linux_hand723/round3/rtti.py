import struct,sys
p="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/dl/game/Fable The Lost Chapters/Fable.exe"
d=open(p,'rb').read()
pe=struct.unpack_from('<I',d,0x3c)[0]
n=struct.unpack_from('<H',d,pe+6)[0]; opt=struct.unpack_from('<H',d,pe+20)[0]
secs=[]
for i in range(n):
    o=pe+24+opt+40*i
    vs,va,rs,ra=struct.unpack_from('<IIII',d,o+8); secs.append((va+0x400000,vs,ra,rs))
def rd(a,k=4):
    for va,vs,ra,rs in secs:
        if va<=a<va+max(vs,rs): return d[a-va+ra:a-va+ra+k]
def u32(a): return struct.unpack('<I',rd(a))[0]
def name(td):
    s=rd(td+8,200); return s[:s.index(b'\0')].decode()
vt=int(sys.argv[1],16); col=u32(vt-4)
td=u32(col+12); chd=u32(col+16)
print('class',name(td))
nb=u32(chd+8); bca=u32(chd+12)
for i in range(nb):
    bcd=u32(bca+4*i); print(' base',name(u32(bcd)), 'mdisp',struct.unpack('<i',rd(bcd+8))[0])
