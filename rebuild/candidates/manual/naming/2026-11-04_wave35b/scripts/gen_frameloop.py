# gen_frameloop.py <outdir>: the 11 `do { Game->NewScriptFrame(); } while (!IsActiveThreadTerminating());` Mains (landed as free __fastcall stand-ins)
import sys,re,json,os
from common35 import *
from entmodel import model
from lvl import datatype
out=os.path.abspath(sys.argv[1]); os.makedirs(out,exist_ok=True)
ent=ps1_entries(); cat=catalog(); C=cats(); EV=evidence()
res=[]
for a,(addr,k,sym,t,name) in sorted(C.items()):
    if k!='FREE' or name.endswith('::Alloc'): continue
    classes,meth=split_target(name)
    ln=int(cat[a][2]); evf,evs=EV.get(a,('',''))
    loop='    do {\n        Game->NewScriptFrame();\n    } while (!IsActiveThreadTerminating());'
    if len(classes)==2:
        outer,inner=classes
        text,aa,asrc,q=model(inner,outer,meth)
        if text is None: res.append((a,'DEFER',q)); continue
        if 'engine/CGameScriptInterfaceBase.h' not in text and 'engine/CGameScriptInterface.h' not in text:
            text=text.replace('#include "engine/CEntityScriptBase.h"','#include "engine/CEntityScriptBase.h"\n// CGameScriptInterfaceBase: the retail interface (slot 7 NewScriptFrame, +0x1c).\n#include "engine/CGameScriptInterfaceBase.h"',1)
        text=text.replace('/*@@DEF@@*/','void %s::%s()\n{\n%s\n}'%(q,meth,loop))
        model_l=['// Class model: the landed Alloc row 0x%s (rebuild/src/compiled/%s) as wave 33 D1 emits from it: its user-declared'%(aa.upper(),asrc),
                 '//   destructor dropped, Main private virtual as the PDB spells it; the Alloc body is not part of this TU.']
        calls='Game (+0x04) slot 7 NewScriptFrame (+0x1c), CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30'
    else:
        X=classes[0]; T=datatype(X) or 'CQ_SunnyvaleMasterData'
        text='\n'.join(['#define FABLE_RETAIL_NSCRIPT','#include "engine/CLevelScriptBase.h"','#include "engine/CGameScriptInterfaceBase.h"','namespace NScript {','class %s;'%T,
            'class %s : public CLevelScriptBase<%s> {'%(X,T),'public:','    virtual void %s();%s// slot 2'%(meth,' '*26),'};','void %s::%s()'%(X,meth),'{',loop,'}','}'])
        model_l=['// Model: engine/CLevelScriptBase.h (Game +0x40) + engine/CGameScriptInterfaceBase.h; Main public virtual as the PDB spells it.']
        calls='Game (+0x40) slot 7 NewScriptFrame (+0x1c), CScriptBase::IsActiveThreadTerminating 0x00CB7940'
    hdr=['// %s() -- run script frames until the thread is told to terminate'%name,'// Leaf: %s'%t,
         '// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %d bytes, thiscall, plain `ret`; calls %s.'%(a.upper(),ln,calls),
         '// Identity: %s (%s); PDB %s in %s.'%(short_ev(evs),'/'.join(evf.split('/')[-2:]),t,pdbwhere(t)),
         '// Naming wave 35: was landed as the free __fastcall stand-in `%s` on a pad model (%s); same bytes.'%(sym,ent[a]['Source'])]+model_l
    newmod='NScript::'+'::'.join(classes); base=fname(newmod)[:80]+'_'+meth+'_'+a
    nsrc=os.path.dirname(ent[a]['Source'])+'/'+base+'.cpp'; ntest=os.path.dirname(ent[a]['TestSource'])+'/'+base+'_test.cpp'
    p=os.path.join(out,'src',nsrc); os.makedirs(os.path.dirname(p),exist_ok=True)
    open(p,'w').write('\n'.join(hdr)+'\n'+text.rstrip('\n')+'\n')
    res.append((a,'OK',json.dumps(dict(path=p,leaf=t,module=newmod,new_src=nsrc,new_test=ntest,old_src=ent[a]['Source'],old_test=ent[a]['TestSource'],old_sym=sym,fam='frameloop'))))
with open(out+'/gen.tsv','w') as f:
    for r in res: f.write('\t'.join(r)+'\n')
import collections; print(collections.Counter(r[1] for r in res))
for r in res:
    if r[1]!='OK': print(r)
