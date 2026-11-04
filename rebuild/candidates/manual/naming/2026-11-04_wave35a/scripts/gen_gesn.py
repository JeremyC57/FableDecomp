# gen_gesn.py <outdir>: CEntityScriptBinding<T>::GetEntityScriptName from the landed slot-1 row's TU (explicit instantiation)
import sys,os,re,json,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from lib35 import *
ent=ps1_entries(); cat=catalog(); EV=evidence(); P=pdbmap()
outdir=sys.argv[1]; only=set(sys.argv[2:]); os.makedirs(outdir,exist_ok=True)
byname={}
for a,q in cat.items(): byname.setdefault(q[1],[]).append(a)
res=[]
for l in list(open(W+'/ROWS.tsv'))[1:]:
    a,cn,leaf,src=l.rstrip('\n').split('\t')
    m=re.match(r'^CEntityScriptBinding<NScript::(\w+)>::GetEntityScriptName$',cn)
    if not m or (only and a not in only): continue
    T=m.group(1); qual='CEntityScriptBinding<NScript::%s>'%T
    sym='?GetEntityScriptName@?$CEntityScriptBinding@V%s@NScript@@@@UBEABVCCharString@@XZ'%T
    if cat[a][3].lower()!='8d4104c3': res.append((a,'DEFER','body is not lea eax,[ecx+4]; ret')); continue
    if sym not in P: res.append((a,'DEFER','PDB lacks '+sym)); continue
    ev=EV.get(a)
    if not ev or ev[0]!=cn: res.append((a,'DEFER','no anniversary evidence row with the catalog name')); continue
    s1=[x for x in byname.get(qual+'::CreateActiveEntityScript',[]) if x in ent]
    s1t=(' (slot 1 CreateActiveEntityScript is the landed row 0x%s)'%s1[0].upper()) if len(s1)==1 else ''
    tu='''// CCharString / CCountedPointer<T> / CScriptThing first (the engine/CEntityScriptBinding.h contract), then the binding template.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBinding.h"
namespace NScript { class %s; }   // the level script; only T* enters the binding's layout
template const CCharString& CEntityScriptBinding<NScript::%s>::GetEntityScriptName() const;
'''%(T,T)
    e=ent[a]; old=open(ROOT+'/rebuild/src/compiled/'+e['Source'],errors='replace').read()
    hdr=['// %s::GetEntityScriptName() const -- the registered entity-script name (EntityScriptName, +0x04)'%qual,
         '// Leaf: %s'%sym,
         '// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, 4 bytes: lea eax,[ecx+4]; ret.'%a.upper(),
         '// Identity (naming wave 35, family gesn): %s.'%ev[1].replace('; LANDED row (respell source)',''),
         '//   %s in %s (%s).'%(sym,pdbs(sym),ev[2]),
         '// Was: %s (catalog stand-in %s).'%(first_comment(old),e['Module']),
         '// Shape: engine/CEntityScriptBinding.h defines the member inline (slot 0 of every binding vtable); this TU instantiates only it.'+s1t]
    new='\n'.join(hdr)+'\n'+tu
    dd=os.path.dirname(e['Source']); base=fname(qual)+'_GetEntityScriptName_'+a
    nsrc=dd+'/'+base+'.cpp'; ntest=os.path.dirname(e['TestSource'])+'/'+base+'_test.cpp'
    p=os.path.join(outdir,'src',nsrc); os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'w').write(new)
    res.append((a,'OK',json.dumps(dict(qual=qual,name='GetEntityScriptName',module=qual,sym=sym,path='M',old_src=e['Source'],new_src=nsrc,old_test=e['TestSource'],new_test=ntest,old_module=e['Module'],cat=cn))))
with open(os.path.join(outdir,'gen.tsv'),'w') as f:
    for x in res: f.write('\t'.join(x)+'\n')
print(collections.Counter((x[1],x[2][:60] if x[1]!='OK' else '') for x in res))
