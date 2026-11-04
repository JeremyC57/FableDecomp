# gen_lvlalloc.py <outdir>: level-script static Alloc rows (CScriptInfoManager::RegisterScripts factories)
import sys,re,json,os
from common35 import *
from retail import disasm
from lvl import *
out=os.path.abspath(sys.argv[1]); os.makedirs(out,exist_ok=True)
ent=ps1_entries(); cat=catalog(); C=cats(); EV=evidence(); E,F=pdb()
res=[]
for a,(addr,k,sym,t,name) in sorted(C.items()):
    if not name.endswith('::Alloc') or t.count('@')<4 or not t.startswith('?Alloc@C') : pass
    if not name.endswith('::Alloc'): continue
    classes,meth=split_target(name)
    if len(classes)!=1: res.append((a,'DEFER','entity Alloc (manual)')); continue
    X=classes[0]
    txt='\n'.join(disasm(a))
    cl=re.findall(r'call\s+0x([0-9a-f]+)',txt)
    sz=int(re.search(r'push\s+0x([0-9a-f]+)\n[^\n]*\n[^\n]*\n[^\n]*call\s+0xbfea1a',txt).group(1),16) if re.search(r'push\s+0x([0-9a-f]+)\n[^\n]*\n[^\n]*\n[^\n]*call\s+0xbfea1a',txt) else None
    if sz is None: res.append((a,'DEFER','size not found')); continue
    T=datatype(X) or 'CQ_SunnyvaleMasterData'
    tnote='' if datatype(X) else ' (CLevelScriptBase<CQ_SunnyvaleMasterData>: the quest-script default, as the 34 shared script headers)'
    dtor='??1%s@NScript@@UAE@XZ'%X
    hasd=dtor in E or dtor in F
    mem=[]; extra=[]
    ctor_sym='??0%s@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z'%X
    if cl[0]!='bfea1a': res.append((a,'DEFER','first call not operator new')); continue
    if len(cl)==2 and cl[1]=='cb8110':
        kind='inline'; cur=0x48
        ctor='    %s(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<%s>(data, game) { }'%(X,T)
    elif len(cl)==3 and cl[1]=='cb8110' and cl[2]=='99e4b0':
        m=re.search(r'lea\s+ecx,\[esi\+0x([0-9a-f]+)\]\n[^\n]*\n[^\n]*\n[^\n]*\n\s*[0-9a-f]+:[^\n]*call\s+0x99e4b0',txt)
        if not m: res.append((a,'DEFER','CCharString member offset not found')); continue
        mo=int(m.group(1),16); em=egor_member_rt(X,mo)
        if not em or em[0]!='CCharString': res.append((a,'DEFER','no Ego_r CCharString member at +0x%x+0x14'%mo)); continue
        kind='inline+CCharString'; 
        if mo>0x48: mem.append('    unsigned char _members48[0x%x - 0x48];%s// +0x48 (the script\'s own members before %s)'%(mo,' '*8,em[1]))
        mem.append('    CCharString %s;%s// +0x%x (Ego_r.pdb +0x%x; default ctor 0x0099E4B0)'%(em[1],' '*max(1,36-len(em[1])),mo,mo+0x14))
        cur=mo+4
        ctor='    %s(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<%s>(data, game) { }'%(X,T)
        extra.append('fable_string')
    elif len(cl)==2:
        ct=cl[1]
        if ctor_sym not in E and ctor_sym not in F: res.append((a,'DEFER','ctor %s not in PDB'%ctor_sym)); continue
        kind='ool'; cur=0x48
        ctor='    %s(CScriptDataBase* data, CGameScriptInterfaceBase& game);%s// 0x%s'%(X,' '*4,ct.upper().zfill(8))
    else: res.append((a,'DEFER','call shape %s'%cl)); continue
    if sz>cur: mem.append('    unsigned char _members%x[0x%x - 0x%x];%s// +0x%x (the script\'s own members; retail sizeof 0x%X)'%(cur,sz,cur,' '*4,cur,sz))
    elif sz<cur: res.append((a,'DEFER','size 0x%x < 0x%x'%(sz,cur))); continue
    es=egor_size(X)
    ln=int(cat[a][2]); evf,evs=EV.get(a,('',''))
    allocpdb=pdbwhere(t)
    hdr=['// %s(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new %s(data, game)'%(name,X),
         '// Leaf: %s'%t,
         '// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %d bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.'%(a.upper(),ln),
         '// Identity: %s; PDB %s in %s.'%(short_ev(evs),t,allocpdb),
         '// Naming wave 35: was landed as the free __fastcall stand-in `%s` (%s); same bytes.'%(sym,ent[a]['Source'])]
    if kind=='ool':
        hdr.append('// Callees: scalar operator new 0x00BFEA1A (0x%X = sizeof), the out-of-line constructor 0x%s (%s, %s).'%(sz,cl[1].upper().zfill(8),ctor_sym,pdbwhere(ctor_sym)))
    else:
        hdr.append('// Callees: scalar operator new 0x00BFEA1A (0x%X = sizeof%s), CScriptBase::CScriptBase 0x00CB8110%s; the constructor is inlined'%(sz,', Ego_r.pdb %d - 0x14'%es if es and es-0x14==sz else '',', CCharString::CCharString() 0x0099E4B0' if 'fable_string' in extra else ''))
        hdr.append('//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).')
    hdr.append('// Model: engine/CLevelScriptBase.h%s; layout = Ego_r.pdb - 0x14 (the retail script base is 0x14 smaller), the members this'%tnote)
    hdr.append('//   body does not touch are one pad block.')
    body=['#define FABLE_RETAIL_NSCRIPT','#include "engine/CLevelScriptBase.h"']
    if 'fable_string' in extra: body.append('#include "fable_string.h"')
    body+=['namespace NScript {','class %s;'%T,'class %s : public CLevelScriptBase<%s> {'%(X,T),'public:',ctor]
    if hasd: body.append('    virtual ~%s();%s// %s'%(X,' '*4,dtor))
    body.append('    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);')
    body+=mem
    body+=['};','CScriptBase* __fastcall %s::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)'%X,'{','    return new %s(data, game);'%X,'}','}',
           'typedef char ExactOwnerSize[sizeof(NScript::%s) == 0x%x ? 1 : -1];'%(X,sz)]
    newmod='NScript::%s'%X
    base=fname(newmod)[:80]+'_Alloc_'+a
    nsrc=os.path.dirname(ent[a]['Source'])+'/'+base+'.cpp'; ntest=os.path.dirname(ent[a]['TestSource'])+'/'+base+'_test.cpp'
    p=os.path.join(out,'src',nsrc); os.makedirs(os.path.dirname(p),exist_ok=True)
    open(p,'w').write('\n'.join(hdr+body)+'\n')
    res.append((a,'OK',json.dumps(dict(path=p,leaf=t,module=newmod,new_src=nsrc,new_test=ntest,old_src=ent[a]['Source'],old_test=ent[a]['TestSource'],old_sym=sym,fam='lvlalloc-'+kind))))
with open(out+'/gen.tsv','w') as f:
    for r in res: f.write('\t'.join(r)+'\n')
import collections; print(collections.Counter(r[1] for r in res))
for r in res:
    if r[1]!='OK': print(r)
