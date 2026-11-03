# seg.py <addr> <lenhex> <obj> <leaf>: per-segment byte deltas between consecutive aligned calls (retail vs ours)
import sys,subprocess,re,os,difflib
sys.path.insert(0,os.path.dirname(__file__))
from coff import load
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import importlib.util
addr,ln,obj,leaf=sys.argv[1:5]
S=os.environ['S']
spec=importlib.util.spec_from_file_location('cl',os.path.dirname(__file__)+'/calls.py')
src=open(os.path.dirname(__file__)+'/calls.py').read()
kdef=src[src.index('def k(s):'):src.index('a=[k(x)')]
ns={'re':re}; exec(kdef,ns); k=ns['k']
def vt(op):
    m=re.search(r'\[e..(?: \+ (0x[0-9a-f]+))?\]',op); return 'V+'+(m.group(1) or '0x0') if m else op
rl=subprocess.run(['python3',S+'/w22/agent724b/ann.py',addr,ln],capture_output=True,text=True,env=dict(os.environ,S=S)).stdout.splitlines()
ret=[]
for l in rl:
    p=l.split(None,2)
    if len(p)>2 and p[1]=='call':
        m=re.match(r'(0x[0-9a-f]+)(?: ; (.*))?$',p[2])
        ret.append((int(p[0],16)-int(addr,16),(m.group(2) or m.group(1)) if m else vt(p[2])))
n,code,rels=load(obj,leaf)
ours=[]
for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(code,0):
    if i.mnemonic=='call':
        ours.append((i.address,rels.get(i.address+1,vt(i.op_str)) if i.bytes[0]==0xe8 else vt(i.op_str)))
a=[k(x[1]) for x in ret]; b=[k(x[1]) for x in ours]
sm=difflib.SequenceMatcher(None,a,b,autojunk=False)
pr=(0,0)
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag!='equal': continue
    for d in range(i2-i1):
        r=ret[i1+d][0]; o=ours[j1+d][0]
        dr=r-pr[0]; do=o-pr[1]
        if dr!=do: print(f'retail {int(addr,16)+pr[0]:08x}..{int(addr,16)+r:08x} {dr:5d}  ours {pr[1]:05x}..{o:05x} {do:5d}  delta {do-dr:+d}  [{a[i1+d]}]')
        pr=(r,o)
