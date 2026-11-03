# calls.py <addr> <lenhex> <obj> <leafsubstr>: retail call targets vs our call relocation symbols (difflib-aligned)
import sys,subprocess,re,os,difflib
sys.path.insert(0,os.path.dirname(__file__))
from coff import load
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
addr,ln,obj,leaf=sys.argv[1:5]
S=os.environ['S']
rl=subprocess.run(['python3',S+'/w22/agent724b/ann.py',addr,ln],capture_output=True,text=True,env=dict(os.environ,S=S)).stdout.splitlines()
def vt(op):
    m=re.search(r'\[e..(?: \+ (0x[0-9a-f]+))?\]',op); return 'V+'+(m.group(1) or '0x0') if m else op
ret=[]
for l in rl:
    p=l.split(None,2)
    if len(p)>2 and p[1]=='call':
        m=re.match(r'(0x[0-9a-f]+)(?: ; (.*))?$',p[2])
        ret.append((m.group(2) or m.group(1)) if m else vt(p[2]))
n,code,rels=load(obj,leaf)
ours=[]
for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(code,0):
    if i.mnemonic=='call':
        ours.append(rels.get(i.address+1,vt(i.op_str)) if i.bytes[0]==0xe8 else vt(i.op_str))
def k(s):
    if s.startswith('V+'): return s
    if s.startswith('?') or s.startswith('_'):
        m=re.match(r'\?{1,2}\$?([^@]+)',s); t=m.group(1) if m else s
        t={'0':'ctor','1':'dtor','A':'operator[]','2':'opnew','3':'opdel','4':'operator=','8':'operator=='}.get(t[0],t) if len(t)>0 and t[0] in '012348A' and s.startswith('??') else t
        t=t.lstrip('_').lower()
        return {'copy':'copy','find':'find','free':'free','malloc':'malloc'}.get(t,t)
    AM={'0x5fbaa0':'getrelativeposonminimap','0x5faba0':'isthingasposbeingdrawn','0x5fe840':'constructregionexitmarkers','0x429c6f':'reset'}
    if s in AM: return AM[s]
    if 'IsPositionInsideClipEdges' in s: return 'isthingasposbeingdrawn'
    if 'Copy_UIMiniMapMarker' in s: return 'copy'
    if 'Find_00612db0' in s: return 'find'
    while True:
        s2=re.sub(r'<[^<>]*>','',s)
        if s2==s: break
        s=s2
    t=s.split('::')[-1].split('(')[0].strip()
    if t.startswith('~'): return 'dtor'
    if t=='operator[]': return 'operator[]'
    if t=='operator new': return 'opnew'
    if t=='operator_delete': return 'opdel'
    if t=='operator=': return 'operator='
    t=t.lstrip('_')
    if s.split('::')[-1]==s.split('::')[-2:][0] and '::' in s: return 'ctor'
    return t.lower()
a=[k(x) for x in ret]; b=[k(x) for x in ours]
sm=difflib.SequenceMatcher(None,a,b,autojunk=False)
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal': continue
    print(f'@@ {tag} retail[{i1}:{i2}] ours[{j1}:{j2}]')
    for x in ret[i1:i2]: print('  - '+x[:100])
    for x in ours[j1:j2]: print('  + '+x[:100])
print('retail calls',len(ret),'ours',len(ours))
