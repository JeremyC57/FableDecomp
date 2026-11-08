#!/usr/bin/env python3
"""rs.py <va> [...]: print the C string at each VA of Fable.exe."""
import sys, struct
P='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/dl/game/Fable The Lost Chapters/Fable.exe'
d=open(P,'rb').read()
pe=struct.unpack_from('<I',d,0x3c)[0]; n=struct.unpack_from('<H',d,pe+6)[0]; opt=struct.unpack_from('<H',d,pe+20)[0]
base=struct.unpack_from('<I',d,pe+24+28)[0]; sec=pe+24+opt
secs=[struct.unpack_from('<8sIIII',d,sec+40*i) for i in range(n)]
def off(va):
    rva=va-base
    for name,vs,va_,rs,ro in secs:
        if va_<=rva<va_+max(vs,rs): return rva-va_+ro
for a in sys.argv[1:]:
    o=off(int(a,16)); e=d.index(b'\0',o); print(a, repr(d[o:e].decode('latin1')))
