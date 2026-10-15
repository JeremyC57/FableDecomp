# usage: python3 split_rows.py <addr>:<len> ...        (repo root, S exported)
# Library + CLI for skipped chunk rows (see rebuild/candidates/manual/SKIPPED_ROWS.tsv): finds the real functions inside
# a row straight from Fable.exe. A function's extent = a linear sweep from its start that tracks forward branch targets
# and stops at the first ret/jmp (or int3) that no earlier branch reaches past; int3/nop/`lea ecx,[ecx]` padding between
# functions is skipped. kind = multi (the row holds several functions), prefix (the row is the start of a longer
# function), single (the row is exactly one function).
import os,struct,sys
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
S=os.environ['S']
_exe=open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
_pe=struct.unpack_from("<I",_exe,0x3c)[0]; _n=struct.unpack_from("<H",_exe,_pe+6)[0]; _o=struct.unpack_from("<H",_exe,_pe+20)[0]
_secs=[]
for i in range(_n):
    o=_pe+24+_o+40*i; vs,va,rs,ro=struct.unpack_from("<IIII",_exe,o+8); _secs.append((va,rs,ro))
def rd(va,n):
    for sva,rs,ro in _secs:
        if sva<=va-0x400000<sva+rs: off=va-0x400000-sva+ro; return _exe[off:off+n]
    return b''
_md=Cs(CS_ARCH_X86,CS_MODE_32)
def extent(start, limit=0x4000):
    far=start
    for ins in _md.disasm(rd(start,limit),start):
        m=ins.mnemonic
        if m.startswith('j'):
            try:
                t=int(ins.op_str,16)
                if t>far and t<start+limit: far=t
            except ValueError: pass
        if m=='int3' and ins.address>=far: return ins.address
        if m in ('ret','retn','jmp') and ins.address+ins.size>far: return ins.address+ins.size
    return None
def pad_skip(a):
    b=rd(a,64); i=0
    while i<len(b):
        if b[i] in (0xcc,0x90): i+=1
        elif b[i:i+3]==b'\x8d\x49\x00': i+=3
        elif b[i:i+6]==b'\x8d\x9b\x00\x00\x00\x00' or b[i:i+7]==b'\x8d\xa4\x24\x00\x00\x00\x00': i+=6 if b[i+1]==0x9b else 7
        else: break
    return a+i
def pieces(start, rowlen):
    """[(addr, len)] of the functions that START inside [start, start+rowlen); the last one may run past the row."""
    out=[]; a=start
    while a<start+rowlen:
        e=extent(a)
        if e is None: break
        out.append((a,e-a)); a=pad_skip(e)
    return out
def classify(start, rowlen):
    p=pieces(start,rowlen)
    if not p: return 'unknown',p
    if len(p)==1: return ('prefix' if p[0][1]>rowlen else 'single'),p
    return 'multi',p
if __name__=='__main__':
    cat={l.split('\t')[0].lower() for l in open('rebuild/oracles/auto-re-candidates.tsv')}
    for arg in sys.argv[1:]:
        a,l=arg.split(':'); k,p=classify(int(a,16),int(l))
        print(a, k, ' '.join('%08x:%d%s'%(x,n,'(landed)' if '%08x'%x in cat else '') for x,n in p))
