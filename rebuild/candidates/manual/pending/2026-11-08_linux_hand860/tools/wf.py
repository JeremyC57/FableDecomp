# wf.py <va>... : print the float at each retail VA
import sys,os,struct
S=os.environ['S']; b=open(S+'/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
pe=struct.unpack_from('<I',b,0x3c)[0]; n=struct.unpack_from('<H',b,pe+6)[0]; opt=struct.unpack_from('<H',b,pe+20)[0]
secs=[struct.unpack_from('<8sIIII',b,pe+24+opt+40*i) for i in range(n)]
def off(va):
    rva=va-0x400000
    for nm,vs,va_,rs,ro in secs:
        if va_<=rva<va_+max(vs,rs): return rva-va_+ro
for a in sys.argv[1:]:
    o=off(int(a,16)); print(a, struct.unpack_from('<f',b,o)[0], hex(struct.unpack_from('<I',b,o)[0]))
