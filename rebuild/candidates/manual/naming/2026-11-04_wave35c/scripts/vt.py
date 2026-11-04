# vt.py addr...: dword refs of each address; for each ref, print slot candidates (slot0 catalog name for slot k=0..8)
import sys
sys.path.insert(0,'/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/wave35c/scripts')
from c35 import *
cat=catalog()
for a in sys.argv[1:]:
    refs,p=dword_refs(int(a,16))
    q=cat.get(a,['','?','',''])
    print(a,q[1],q[2],q[3][:40],'refs',['%08x'%r for r in refs])
    for r in refs:
        for k in range(0,10):
            s0=p.u32(r-4*k); 
            if s0 is None: continue
            n=cat.get('%08x'%s0,[0,''])[1]
            if 'scalar deleting' in n: print('   slot',k,'of vt %08x'%(r-4*k),'slot0',n)
