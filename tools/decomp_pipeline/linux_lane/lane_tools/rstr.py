import sys,struct
d=open('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
pe=struct.unpack_from('<I',d,0x3c)[0]; n=struct.unpack_from('<H',d,pe+6)[0]; opt=struct.unpack_from('<H',d,pe+20)[0]
secs=[]
for i in range(n):
    o=pe+24+opt+i*40; vs,va,rs,ro=struct.unpack_from('<IIII',d,o+8); secs.append((0x400000+va,vs,ro))
def rd(va,n=200):
    for b,vs,ro in secs:
        if b<=va<b+vs: return d[ro+va-b:ro+va-b+n]
for a in sys.argv[1:]:
    va=int(a,16); s=rd(va); print(hex(va), repr(s.split(b'\0')[0][:150]), s[:8].hex())
