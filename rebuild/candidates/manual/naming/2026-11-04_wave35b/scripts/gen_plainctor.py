# gen_plainctor.py <outdir>: the 27-byte CGlobal_* / CExpression_* constructors X(CGameScriptInterfaceBase& game)
import sys,re,json,os
from common35 import *
from retail import disasm
out=os.path.abspath(sys.argv[1]); os.makedirs(out,exist_ok=True)
ent=ps1_entries(); cat=catalog(); C=cats(); EV=evidence(); E,F=pdb()
res=[]
for a,(addr,k,sym,t,name) in sorted(C.items()):
    if k!='CTOR' or not t.endswith('QAE@AAVCGameScriptInterfaceBase@@@Z'): continue
    classes,meth=split_target(name); X=classes[0]
    ins=[l.split('\t')[-1].strip() for l in disasm(a)]
    vt=re.search(r'mov\s+DWORD PTR \[esi\],0x([0-9a-f]+)',' ; '.join(ins))
    if len(ins)!=9 or 'call   0xcb8110' not in ins or not vt: res.append((a,'DEFER','ctor shape')); continue
    vt=vt.group(1)
    dtor='??1%s@NScript@@UAE@XZ'%X; hasd=dtor in E or dtor in F
    ln=int(cat[a][2]); evf,evs=EV.get(a,('',''))
    hdr=['// %s(CGameScriptInterfaceBase& game) -- CScriptBase(), Game = &game, then the script vtable 0x%s'%(name,vt.upper().zfill(8)),
         '// Leaf: %s'%t,
         '// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %d bytes, thiscall, `ret 4`; calls CScriptBase::CScriptBase 0x00CB8110.'%(a.upper(),ln),
         '// Identity: %s (%s); PDB %s in %s.'%(short_ev(evs),'/'.join(evf.split('/')[-2:]),t,pdbwhere(t)),
         '// Naming wave 35: was landed as the stand-in `%s` (%s) on a CBase_00cb8110 base; same bytes.'%(sym,ent[a]['Source']),
         '// Model: engine/CScriptBase.h; a global script has no master data (PDB ctor takes only the interface): Game +0x40.']
    b=['#define FABLE_RETAIL_NSCRIPT','#include "engine/CScriptBase.h"','class CGameScriptInterfaceBase;','namespace NScript {',
       'class %s : public CScriptBase {'%X,'public:','    %s(CGameScriptInterfaceBase& game);'%X]
    if hasd: b.append('    virtual ~%s();%s// %s'%(X,' '*4,dtor))
    b+=['    CGameScriptInterfaceBase* Game;%s// +0x40'%(' '*9),'};','%s::%s(CGameScriptInterfaceBase& game) : Game(&game)'%(X,X),'{','}','}']
    newmod='NScript::%s'%X; base=fname(newmod)[:80]+'_'+X[:48]+'_'+a
    nsrc=os.path.dirname(ent[a]['Source'])+'/'+base+'.cpp'; ntest=os.path.dirname(ent[a]['TestSource'])+'/'+base+'_test.cpp'
    p=os.path.join(out,'src',nsrc); os.makedirs(os.path.dirname(p),exist_ok=True)
    open(p,'w').write('\n'.join(hdr+b)+'\n')
    res.append((a,'OK',json.dumps(dict(path=p,leaf=t,module=newmod,new_src=nsrc,new_test=ntest,old_src=ent[a]['Source'],old_test=ent[a]['TestSource'],old_sym=sym,fam='plainctor'))))
with open(out+'/gen.tsv','w') as f:
    for r in res: f.write('\t'.join(r)+'\n')
import collections; print(collections.Counter(r[1] for r in res))
for r in res:
    if r[1]!='OK': print(r)
