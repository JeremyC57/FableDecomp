# xr.py <hexva>... : direct call/jmp rel32 sites targeting each VA in Fable.exe .text
import struct,sys
f=open("${S}/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
pe=struct.unpack_from("<I",f,0x3c)[0]; ns=struct.unpack_from("<H",f,pe+6)[0]; so=struct.unpack_from("<H",f,pe+20)[0]
o=pe+24+so; vs,va,rs,ra=struct.unpack_from("<IIII",f,o+8)
text=f[ra:ra+rs]; base=0x400000+va
ts=[int(a,16) for a in sys.argv[1:]]
for i in range(len(text)-5):
    if text[i] in (0xe8,0xe9):
        d=struct.unpack_from("<i",text,i+1)[0]; t=base+i+5+d
        if t in ts: print(hex(t), hex(base+i), 'call' if text[i]==0xe8 else 'jmp')
