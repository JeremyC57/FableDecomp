# rx.py dis <addr> [n] | vt <vtable> [nslots] | refs <addr> : retail helpers (catalog bytes / PE)
import sys,os,subprocess,struct
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from vcommon import *
sys.path.insert(0,'/home/user/FableDecomp/rebuild/candidates/manual/naming/2026-11-03_wave33/scripts')
cat=catalog()
cmd=sys.argv[1]
def nm(v): 
    q=cat.get('%08x'%v); return q[1] if q else ''
if cmd=='dis':
    a=sys.argv[2].lower(); n=int(sys.argv[3]) if len(sys.argv)>3 else 60
    q=cat[a]; b=bytes.fromhex(q[3].strip()); f=W+'/cc/_dis.bin'; open(f,'wb').write(b)
    o=subprocess.run(['objdump','-D','-b','binary','-mi386','-Mintel','--adjust-vma=0x'+a,f],capture_output=True,text=True).stdout.splitlines()[7:]
    import re
    for l in o[:n]:
        m=re.search(r'(call|jmp)\s+0x([0-9a-f]+)',l)
        extra=('   ; '+nm(int(m.group(2),16))) if m else ''
        print(l+extra)
    print('#',q[1],q[2],'bytes')
else:
    from pe import PE
    p=PE(os.environ.get('FABLE_EXE') or '/home/user/FableDecomp/Fable.exe')
    if cmd=='vt':
        v=int(sys.argv[2],16); n=int(sys.argv[3]) if len(sys.argv)>3 else 30
        for k in range(n):
            x=p.u32(v+4*k)
            if x is None: break
            print(k,'+%x'%(4*k),'%08x'%x,nm(x) if p.is_text(x) else '(non-text)')
    elif cmd=='refs':
        t=int(sys.argv[2],16); pat=struct.pack('<I',t)
        i=0
        while True:
            i=p.d.find(pat,i)
            if i<0: break
            for s in p.secs:
                if s[3]<=i<s[3]+s[4]: print(s[0],'%08x'%(s[1]+i-s[3]))
            i+=1
