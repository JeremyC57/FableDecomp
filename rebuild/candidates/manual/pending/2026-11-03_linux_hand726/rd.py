import sys,struct
exe=open("/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/dl/game/Fable The Lost Chapters/Fable.exe",'rb').read()
pe=struct.unpack_from('<I',exe,0x3c)[0]; nsec=struct.unpack_from('<H',exe,pe+6)[0]; opt=struct.unpack_from('<H',exe,pe+20)[0]
def off(v):
    for i in range(nsec):
        o=pe+24+opt+40*i; vs,va,rs,ra=struct.unpack_from('<IIII',exe,o+8)
        if va<=v-0x400000<va+max(vs,rs): return ra+v-0x400000-va
for a in sys.argv[1:]:
    v=int(a,16); o=off(v); b=exe[o:o+8]
    print(a, b.hex(), struct.unpack('<f',b[:4])[0], struct.unpack('<d',b)[0])
