# xref.py <target-va>...: find E8/E9 rel32 callers of targets in Fable.exe .text; print site and preceding push imm
import sys,struct,os
exe=open(os.environ.get("FABLE_EXE","/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/dl/game/Fable The Lost Chapters/Fable.exe"),'rb').read()
pe=struct.unpack_from('<I',exe,0x3c)[0]; nsec=struct.unpack_from('<H',exe,pe+6)[0]; opt=struct.unpack_from('<H',exe,pe+20)[0]
base=0x400000; secs=[]
for i in range(nsec):
    o=pe+24+opt+40*i; name=exe[o:o+8].rstrip(b'\0'); vs,va,rs,ra=struct.unpack_from('<IIII',exe,o+8); secs.append((name,va,vs,ra,rs))
t=[s for s in secs if s[0]==b'.text'][0]; _,va,vs,ra,rs=t
data=exe[ra:ra+rs]
tg=[int(x,16) for x in sys.argv[1:]]
for i in range(len(data)-5):
    if data[i] in (0xE8,0xE9):
        d=struct.unpack_from('<i',data,i+1)[0]; site=base+va+i; dst=site+5+d
        if dst in tg: print(hex(site), 'call' if data[i]==0xE8 else 'jmp', hex(dst), data[max(0,i-24):i].hex())
