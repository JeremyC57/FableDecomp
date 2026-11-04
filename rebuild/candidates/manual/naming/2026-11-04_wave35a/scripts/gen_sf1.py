# gen_sf1.py <outdir>: CSpawnedFunc1<T,P1>::`scalar deleting destructor' (no member destructor) via the wave 29 sfg MakeOne emitter
import sys,os,re,json,collections,struct
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from lib35 import *
ent=ps1_entries(); cat=catalog(); EV=evidence(); P=pdbmap()
outdir=sys.argv[1]; os.makedirs(outdir,exist_ok=True)
MP={'long':'J','CScriptThing&':'AAVCScriptThing@@'}
res=[]
for l in list(open(W+'/ROWS.tsv'))[1:]:
    a,cn,leaf,src=l.rstrip('\n').split('\t')
    m=re.match(r"^CSpawnedFunc1<NScript::(\w+),([\w&]+)>::`scalar deleting destructor'$",cn)
    if not m: continue
    T,P1=m.groups(); qual='CSpawnedFunc1<NScript::%s,%s>'%(T,P1)
    inst='?$CSpawnedFunc1@V%s@NScript@@%s@@'%(T,MP[P1])
    gsym='??_G%sUAEPAXI@Z'%inst
    pd=['%s %s'%(pdbs(s),s) for s in ('??_G%sUAEPAXI@Z'%inst,'??_E%sUAEPAXI@Z'%inst,'??1%sUAE@XZ'%inst) if s in P]
    if not pd: res.append((a,'DEFER','no PDB ??_G/??_E/??1 for '+qual)); continue
    b=bytes.fromhex(cat[a][3]); base=int(a,16)
    calls=call_targets(a)
    if len(b)!=30 or 0x00cdd4c0 not in calls or 0x00bfe9bc not in calls: res.append((a,'DEFER','calls %s'%calls)); continue
    tu='''#include "fable_string.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CScriptThing.h"
#include "engine/CScriptBase.h"
#include "engine/CSpawnedFunc1.h"
namespace NScript {
class %s : public CScriptBase { };
}

CSpawnedFuncBase* MakeOne(const CCharString& name, unsigned long id, NScript::%s* pclass, void (NScript::%s::*func)(%s), %s par1)
{
    return new CSpawnedFunc1<NScript::%s, %s>(name, id, pclass, func, par1);
}
'''%(T,T,T,P1,P1,T,P1)
    st_,_=qcheck(a,tu,gsym)
    if not st_.startswith(('MATCH','RELOCATION')): res.append((a,'DEFER','MakeOne TU: '+st_)); continue
    ev=EV[a]; e=ent[a]
    hdr=["// %s::`scalar deleting destructor'(unsigned int flags) -- the implicit ~CSpawnedFunc1 (no member destructor: Par1 is %s), i.e. the CSpawnedFuncBase base; free when bit 0 is set"%(qual,P1),
         "// Leaf: %s"%gsym,
         "// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, 30 bytes: push esi; mov esi,ecx; call ~CSpawnedFuncBase 0x00CDD4C0; test byte [esp+8],1; je;"%a.upper(),
         "//   push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.",
         "// Identity (naming wave 35, family sf1): %s."%ev[1],
         "//   %s; PDB: %s."%(ev[2],'; '.join(pd)),
         "//   Was: %s (catalog stand-in %s, Leaf %s)."%(first_comment(open(ROOT+'/rebuild/src/compiled/'+e['Source'],errors='replace').read()),e['Module'],leaf or 'none'),
         "// Shape: the wave 29 sfg emitter (DECISIONS_W29_RESOLVED D7, 26 landed CSpawnedFunc1 ??_G rows): MakeOne constructs the class, so the TU",
         "//   emits the vtable and its ??_G; the implicit ~CSpawnedFuncBase (engine/CSpawnedFuncBase.h) is called out of line."]
    new='\n'.join(hdr)+'\n'+tu
    dd=os.path.dirname(e['Source']); base_=fname(qual)+'_scalardeletingdestructor_'+a
    nsrc=dd+'/'+base_+'.cpp'; ntest=os.path.dirname(e['TestSource'])+'/'+base_+'_test.cpp'
    p=os.path.join(outdir,'src',nsrc); os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'w').write(new)
    res.append((a,'OK',json.dumps(dict(qual=qual,name="`scalar deleting destructor'",module=qual,sym=gsym,path='sf1',old_src=e['Source'],new_src=nsrc,old_test=e['TestSource'],new_test=ntest,old_module=e['Module'],cat=cn))))
with open(os.path.join(outdir,'gen.tsv'),'w') as f:
    for x in res: f.write('\t'.join(x)+'\n')
print(collections.Counter(x[1] for x in res)); [print(x) for x in res if x[1]=='DEFER']
