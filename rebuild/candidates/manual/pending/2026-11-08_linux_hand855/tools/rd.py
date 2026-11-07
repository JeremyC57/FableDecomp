import sys,struct,os
EXE=os.environ["S"]+"/dl/game/Fable The Lost Chapters/Fable.exe"
d=open(EXE,"rb").read(); pe=struct.unpack_from("<I",d,0x3c)[0]; nsec=struct.unpack_from("<H",d,pe+6)[0]; optsz=struct.unpack_from("<H",d,pe+20)[0]
secs=[]
for i in range(nsec):
    o=pe+24+optsz+40*i; vsz,va,rsz,raw=struct.unpack_from("<IIII",d,o+8); secs.append((va+0x400000,vsz,raw,rsz))
def rd(va,n):
    for v,vs,raw,rs in secs:
        if v<=va<v+max(vs,rs): return d[raw+va-v:raw+va-v+n]
for a in sys.argv[1:]:
    t='f'
    if ':' in a: a,t=a.split(':')
    va=int(a,16)
    if t=='f': print(a, struct.unpack('<f',rd(va,4))[0])
    elif t=='d': print(a, struct.unpack('<d',rd(va,8))[0])
    elif t=='s': b=rd(va,200); print(a, repr(b[:b.index(b'\0')]))
    elif t=='w': b=rd(va,400); s=b.decode('utf-16-le'); print(a, repr(s[:s.index(chr(0))]))
    elif t=='x': print(a, rd(va,64).hex())
