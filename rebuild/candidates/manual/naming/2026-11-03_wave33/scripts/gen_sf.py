# sf family: CSpawnedFunc<T> ??_G rows (body: ~CSpawnedFuncBase + delete); T = the class of every landed spawning storer
import sys, os, re, json, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import *
ent=ps1_entries(); cat=catalog(); E,F=pdb()
outdir=sys.argv[1]; only=set(sys.argv[2:])
os.makedirs(outdir,exist_ok=True)
def srcof(a):
    e=ent.get(a)
    if not e: return None
    try: return open(ROOT+'/rebuild/src/compiled/'+e['Source'],encoding='utf-8').read()
    except Exception: return None
def leafof(s):
    m=re.search(r'^//\s*Leaf:\s*(\S+)',s,re.M) if s else None
    return m.group(1) if m else None
res=[]
for l in open(os.environ['S']+'/wave33/famA_tr.tsv'):
    addr,vt,calls,st=l.rstrip('\n').split('\t')
    if calls!='calls=00cdd4c0=CSpawnedFuncBase::~CSpawnedFuncBase ; 00bfe9bc=MSVCR71.DLL::operator_delete': continue
    if only and addr not in only: continue
    sts=[x.split('=',1)[0] for x in st.split(' ; ')] if st else []
    Ts=collections.Counter(); ev=[]; unl=[]
    for s in sts:
        src=srcof(s); lf=leafof(src)
        if not lf: unl.append(s); continue
        if lf.startswith('??0') : continue   # base/other ctors expanding the CSpawnedFuncBase part
        m=re.match(r'^\?([A-Za-z_0-9]+)@((?:[A-Za-z_0-9]+@)+)@',lf)
        if not m: unl.append(s); continue
        scope=m.group(2)[:-1]
        # the instantiation the storer's source writes
        inst=set(re.findall(r'new\s+CSpawnedFunc\s*<\s*([A-Za-z_:0-9]+)\s*>',src))
        Ts[scope]+=1; ev.append((s,lf,sorted(inst)))
    if len(Ts)!=1:
        res.append((addr,'DEFER','storer classes %s (unlanded/unnamed storers %s)'%(dict(Ts),unl))); continue
    scope=list(Ts)[0]; parts=scope.split('@')
    qual='::'.join(reversed(parts))
    insts=set(i for _,_,ii in ev for i in ii)
    insts_n=set(i if i.startswith('NScript::') or '::' in i else 'NScript::'+i for i in insts)
    if insts and insts_n!={qual}:
        res.append((addr,'DEFER','storer sources instantiate %s, storer class %s'%(sorted(insts),qual))); continue
    mq='V'+scope+'@@'
    tsym='?$CSpawnedFunc@'+mq+'@'
    gsym='??_G'+tsym+'@UAEPAXI@Z'
    pd=[]
    for s in ('??_G'+tsym+'@UAEPAXI@Z','??_E'+tsym+'@UAEPAXI@Z','??1'+tsym+'@UAE@XZ'):
        if s in E: pd.append('Ego_r.pdb '+s)
        if s in F: pd.append('FableWin.pdb '+s)
    ctor=[x for x in (E|F) if x.startswith('??0'+tsym+'@QAE@')]
    if not pd and not ctor: res.append((addr,'DEFER','no PDB CSpawnedFunc<%s> member'%qual)); continue
    if not pd: pd=['PDB ctor '+ctor[0]]
    if len(parts)==2: base='CScriptBase'
    elif len(parts)==3: base='CEntityScriptBase'
    else: res.append((addr,'DEFER','scope depth %s'%scope)); continue
    ln=int(cat[addr][2])
    if base=='CScriptBase':
        decl="namespace NScript {\nclass %s : public CScriptBase { };\n}\n"%parts[0]
        inc='#include "engine/CScriptBase.h"\n'
    else:
        decl="namespace NScript {\nclass %s {\npublic:\n    class %s : public CEntityScriptBase { };\n};\n}\n"%(parts[1],parts[0])
        inc='#include "engine/CEntityScriptBase.h"\n'
    storers='; '.join('0x%s %s'%(s.upper(),lf) for s,lf,_ in ev[:4])+(' (+%d more)'%(len(ev)-4) if len(ev)>4 else '')
    hdr=[
"// CSpawnedFunc<%s>::`scalar deleting destructor'(unsigned int flags) -- the implicit ~CSpawnedFunc (no members with destructors), i.e."%qual,
"//   ~CSpawnedFuncBase, then free the thread object when bit 0 is set",
"// Leaf: %s"%gsym,
"// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %d bytes: push esi; mov esi,ecx; call ~CSpawnedFuncBase 0x00CDD4C0; test byte [esp+8],1; je;"%(addr.upper(),ln),
"//   push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.",
"// Naming wave 33 (family sf): slot 0 of vtable 0x%s (no RTTI), the CSpawnedFunc<T> vtable the landed spawning rows of T store"%vt.upper(),
"//   (%s); %s."%(storers,'; '.join(pd)),
"//   Was a free __fastcall stand-in (catalog %s)."%cat[addr][1],
"// Shape: the compiler's deleting destructor is emitted with the vtable, in a TU that constructs the class: MakeOne is that construction (the",
"//   wave 29 CSpawnedFunc1 precedent 0x00CE6C10). The implicit ~CSpawnedFunc is expanded into it, the implicit ~CSpawnedFuncBase called out of line.",
]
    body='''#include "fable_string.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CScriptThing.h"
%s#include "engine/CSpawnedFunc.h"
%s
CSpawnedFuncBase* MakeOne(const CCharString& name, unsigned long id, %s* pclass, void (%s::*func)())
{
    return new CSpawnedFunc<%s>(name, id, pclass, func);
}
'''%(inc,decl,qual,qual,qual)
    new='\n'.join(hdr)+'\n'+body
    tq='CSpawnedFunc<%s>'%qual
    nsrc=os.path.dirname(ent[addr]['Source'])+'/'+fname(tq)+'_scalardeletingdestructor_'+addr+'.cpp'
    ntest=os.path.dirname(ent[addr]['TestSource'])+'/'+fname(tq)+'_scalardeletingdestructor_'+addr+'_test.cpp'
    pth=os.path.join(outdir,'src',nsrc); os.makedirs(os.path.dirname(pth),exist_ok=True); open(pth,'w').write(new)
    res.append((addr,'OK',json.dumps(dict(qual=tq,name=tq+"::`scalar deleting destructor'",module=tq,sym=gsym,old_src=ent[addr]['Source'],new_src=nsrc,old_test=ent[addr]['TestSource'],new_test=ntest,vt=vt,storers=[(s,lf) for s,lf,_ in ev],pdb=pd,old_module=ent[addr]['Module'],old_name=cat[addr][1]))))
cnt=collections.Counter(json.loads(r[2])['sym'] for r in res if r[1]=='OK')
res=[(r[0],'DEFER','symbol %s would name %d retail addresses'%(json.loads(r[2])['sym'],cnt[json.loads(r[2])['sym']])) if r[1]=='OK' and cnt[json.loads(r[2])['sym']]>1 else r for r in res]
with open(os.path.join(outdir,'gen.tsv'),'w') as f:
    for r in res: f.write('\t'.join(r)+'\n')
print(collections.Counter(r[1] for r in res))
for r in res:
    if r[1]=='DEFER': print(r[0],r[2][:200])
