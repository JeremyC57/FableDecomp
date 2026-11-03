# vt.py <RTTI type name> [first last]: print a class's retail vtable slots
import struct,sys
S='${S}'
exe=open(S+"/dl/game/Fable The Lost Chapters/Fable.exe",'rb').read()
pe=struct.unpack_from('<I',exe,0x3c)[0]; nsec=struct.unpack_from('<H',exe,pe+6)[0]; opt=struct.unpack_from('<H',exe,pe+20)[0]
secs=[]
for i in range(nsec):
    o=pe+24+opt+40*i; vs,va,rs,ra=struct.unpack_from('<IIII',exe,o+8); secs.append((va,vs,rs,ra))
def f2v(f):
    for va,vs,rs,ra in secs:
        if ra<=f<ra+rs: return f-ra+va+0x400000
def v2f(v):
    for va,vs,rs,ra in secs:
        if va<=v-0x400000<va+max(vs,rs): return ra+v-0x400000-va
name=sys.argv[1].encode(); a=int(sys.argv[2]) if len(sys.argv)>2 else 0; b=int(sys.argv[3]) if len(sys.argv)>3 else 200
i=exe.find(name+b'\0'); td=f2v(i)-8; p=0; col=None
while True:
    p=exe.find(struct.pack('<I',td),p)
    if p<0: break
    if struct.unpack_from('<III',exe,p-12)==(0,0,0): col=f2v(p-12); break
    p+=1
q=exe.find(struct.pack('<I',col)); vt=f2v(q)+4
print('vtable',hex(vt))
for s in range(a,b+1):
    t=struct.unpack_from('<I',exe,v2f(vt+4*s))[0]; print(s,hex(t))
