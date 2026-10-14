import sys,struct
d=open('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
pe=struct.unpack_from('<I',d,0x3c)[0]; n=struct.unpack_from('<H',d,pe+6)[0]; opt=struct.unpack_from('<H',d,pe+20)[0]
secs=[]
for i in range(n):
    o=pe+24+opt+i*40; vs,va,rs,ro=struct.unpack_from('<IIII',d,o+8); secs.append((va,vs,ro))
def off(rva):
    for va,vs,ro in secs:
        if va<=rva<va+vs: return ro+rva-va
imp_rva=struct.unpack_from('<I',d,pe+24+104)[0]
names={}
o=off(imp_rva)
while True:
    oft,_,_,name,ft=struct.unpack_from('<IIIII',d,o)
    if ft==0: break
    dll=d[off(name):off(name)+64].split(b'\0')[0].decode()
    t=off(oft or ft); a=ft
    while True:
        e=struct.unpack_from('<I',d,t)[0]
        if e==0: break
        nm='#%d'%(e&0xffff) if e&0x80000000 else d[off(e)+2:off(e)+80].split(b'\0')[0].decode()
        names[0x400000+a]=(dll,nm); t+=4; a+=4
    o+=20
for x in sys.argv[1:]: print(hex(int(x,16)), names.get(int(x,16)))
