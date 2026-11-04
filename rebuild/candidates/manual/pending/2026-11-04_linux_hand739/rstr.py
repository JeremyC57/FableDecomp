import os,sys,struct
EXE=os.environ['S']+'/dl/game/Fable The Lost Chapters/Fable.exe'
b=open(EXE,'rb').read()
pe=struct.unpack_from('<I',b,0x3c)[0]; ns=struct.unpack_from('<H',b,pe+6)[0]; oh=struct.unpack_from('<H',b,pe+20)[0]
secs=[]
for i in range(ns):
    o=pe+24+oh+40*i; vs,va,rs,ro=struct.unpack_from('<IIII',b,o+8); secs.append((va+0x400000,vs,ro))
def off(v):
    for va,vs,ro in secs:
        if va<=v<va+vs: return ro+v-va
def s(v):
    o=off(v); e=b.index(b'\0',o); return b[o:e].decode('latin1')
def f(v): return struct.unpack_from('<f',b,off(v))[0]
mode=sys.argv[1]
for a in sys.argv[2:]:
    v=int(a,16)
    print(a, repr(s(v)) if mode=='s' else f(v))
