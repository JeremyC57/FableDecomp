# callers.py <va>...: find E8/E9 rel32 references and absolute dword references to the VAs in retail Fable.exe
import os,sys,struct
S=os.environ['S']; b=open(S+'/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
pe=struct.unpack_from('<I',b,0x3c)[0]; n=struct.unpack_from('<H',b,pe+6)[0]; oh=struct.unpack_from('<H',b,pe+20)[0]
secs=[struct.unpack_from('<8sIIII',b,pe+24+oh+40*i) for i in range(n)]
for a in sys.argv[1:]:
    t=int(a,16)
    for nm,vs,va,rs,rp in secs:
        base=0x400000+va
        d=b[rp:rp+rs]
        for i in range(len(d)-5):
            if d[i] in (0xe8,0xe9):
                if base+i+5+struct.unpack_from('<i',d,i+1)[0]==t: print(a,nm.rstrip(b'\0').decode(),hex(base+i),'call' if d[i]==0xe8 else 'jmp')
        p=struct.pack('<I',t); j=d.find(p)
        while j>=0:
            print(a,nm.rstrip(b'\0').decode(),hex(base+j),'abs'); j=d.find(p,j+1)
