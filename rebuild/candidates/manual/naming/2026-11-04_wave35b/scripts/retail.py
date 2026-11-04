import struct,subprocess,re
from common35 import catalog
_cat=None
def rbytes(a):
    global _cat
    if _cat is None: _cat=catalog()
    q=_cat[a]; return int(a,16),bytes.fromhex(q[3])
def calls(a):
    va,b=rbytes(a); out=[]
    for i in range(len(b)-4):
        if b[i]==0xe8: out.append((i,(va+i+5+struct.unpack_from('<i',b,i+1)[0])&0xffffffff))
    return out
def disasm(a):
    va,b=rbytes(a)
    open('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/wave35b/scratch/_r.bin','wb').write(b)
    r=subprocess.run(['objdump','-D','-b','binary','-mi386','-Mintel','--adjust-vma=0x%x'%va,'/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/wave35b/scratch/_r.bin'],capture_output=True,text=True).stdout
    return [l for l in r.split('\n') if re.match(r'^\s+[0-9a-f]+:',l)]
