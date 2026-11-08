# ws.py <va>... : print the UTF-16 (or ASCII with -a) string at each retail VA
import sys,os,struct
S=os.environ['S']; b=open(S+'/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
pe=struct.unpack_from('<I',b,0x3c)[0]; n=struct.unpack_from('<H',b,pe+6)[0]; opt=struct.unpack_from('<H',b,pe+20)[0]
secs=[struct.unpack_from('<8sIIII',b,pe+24+opt+40*i) for i in range(n)]
def off(va):
    rva=va-0x400000
    for nm,vs,va_,rs,ro in secs:
        if va_<=rva<va_+max(vs,rs): return rva-va_+ro
asc='-a' in sys.argv
for a in [x for x in sys.argv[1:] if x!='-a']:
    o=off(int(a,16))
    if asc: e=b.index(b'\0',o); print(a,repr(b[o:e].decode('latin1')))
    else:
        e=o
        while b[e:e+2]!=b'\0\0': e+=2
        print(a,repr(b[o:e].decode('utf-16le')))
