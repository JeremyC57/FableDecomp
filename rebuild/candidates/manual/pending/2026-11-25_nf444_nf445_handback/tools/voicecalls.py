#!/usr/bin/env python3
"""voicecalls.py (NF445 check 5): in retail 0x00C3C000..0x00C80000, find virtual calls through slot +8 / +0xC whose object pointer was
loaded from a CCodaSource m_pVoice field (+0x50) or whose vtable is 0x012B8AE0's class family, list the call site, the pushes before
it (argument count) and the enclosing landed row (source file whose address range covers the site)."""
import struct,glob,re,os,bisect
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
S='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'; R='/home/user/FableDecomp'
d=open(S+'/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
pe=struct.unpack_from('<I',d,0x3c)[0]; n=struct.unpack_from('<H',d,pe+6)[0]; so=struct.unpack_from('<H',d,pe+20)[0]
secs=[(struct.unpack_from('<I',d,pe+24+so+i*40+12)[0],struct.unpack_from('<I',d,pe+24+so+i*40+8)[0],struct.unpack_from('<I',d,pe+24+so+i*40+20)[0]) for i in range(n)]
def off(va):
    r=va-0x400000
    for a,s,raw in secs:
        if a<=r<a+s: return raw+r-a
lo,hi=0x00C3C000,0x00C80000
md=Cs(CS_ARCH_X86,CS_MODE_32); md.skipdata=True
ins=list(md.disasm(d[off(lo):off(lo)+(hi-lo)],lo))
lens={}
for l in open(R+'/rebuild/oracles/auto-re-candidates.tsv',encoding='utf-8',errors='replace'):
    p=l.split('\t')
    if len(p)>2 and re.fullmatch(r'[0-9a-f]{8}',p[0]):
        a=int(p[0],16)
        if lo<=a<hi and p[2].isdigit(): lens[a]=(int(p[2]),p[1])
starts=sorted(lens)
src={}
for f in glob.glob(R+'/rebuild/src/compiled/00/c[3-7]/*.cpp'):
    m=re.search(r'_([0-9a-f]{8})\.cpp$',f)
    if m: src[int(m.group(1),16)]=os.path.basename(f)
ss={}
for l in open(R+'/rebuild/oracles/self_symbols.tsv',encoding='utf-8',errors='replace'):
    p=l.rstrip('\n').split('\t')
    if len(p)>2 and re.fullmatch(r'[0-9a-f]{8}',p[0]): ss[int(p[0],16)]=(p[1],p[2])
def owner(va):
    i=bisect.bisect_right(starts,va)-1
    if i<0: return None
    a=starts[i]; return a if va<a+lens[a][0] else None
print('site\tslot\tobject_load\tpushes_before\tenclosing_row\tlanded_source\tself_status')
for j,i in enumerate(ins):
    if i.mnemonic!='call': continue
    m=re.fullmatch(r'dword ptr \[(e\w\w) \+ (0x8|0xc)\]',i.op_str)
    if not m: continue
    vreg=m.group(1); win=ins[max(0,j-14):j]
    # vtable reg loaded from [ecx] (or [obj]) ; obj loaded from [x+0x50]
    objload='-'
    for k in range(len(win)-1,-1,-1):
        w=win[k]
        if w.mnemonic=='mov' and w.op_str.startswith(vreg+', dword ptr ['):
            obj=re.match(vreg+r', dword ptr \[(e\w\w)\]',w.op_str)
            if obj:
                oreg=obj.group(1)
                for q in range(k-1,-1,-1):
                    x=win[q]
                    if x.mnemonic=='mov' and x.op_str.startswith(oreg+', dword ptr ['):
                        objload=x.op_str; break
            break
    if '+ 0x50]' not in objload: continue
    pushes=sum(1 for w in win if w.mnemonic=='push')
    a=owner(i.address)
    print(f'{i.address:08x}\t+{m.group(2)}\t{objload}\t{pushes}\t{("%08x %s"%(a,lens[a][1])) if a else "-"}\t{src.get(a,"-") if a else "-"}\t{"|".join(ss.get(a,("-","-"))) if a else "-"}')
