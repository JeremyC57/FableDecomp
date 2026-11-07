# rf.py <va>...: read float/dword at retail VAs
import os,sys,struct
S=os.environ['S']; b=open(S+'/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
pe=struct.unpack_from('<I',b,0x3c)[0]; n=struct.unpack_from('<H',b,pe+6)[0]; oh=struct.unpack_from('<H',b,pe+20)[0]
secs=[struct.unpack_from('<8sIIII',b,pe+24+oh+40*i) for i in range(n)]
for a in sys.argv[1:]:
    va=int(a,16)-0x400000
    for nm,vs,vaddr,rs,rp in secs:
        if vaddr<=va<vaddr+rs:
            o=rp+va-vaddr; print(a, struct.unpack_from('<f',b,o)[0], hex(struct.unpack_from('<I',b,o)[0]), struct.unpack_from('<d',b,o)[0])
