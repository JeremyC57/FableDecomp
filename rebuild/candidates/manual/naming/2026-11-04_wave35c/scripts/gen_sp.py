# gen_sp.py <famdir>: CSpawnedFunc<T> / CSpawnedFunc1<T,P1>::SuspendableProcess rows
import sys, os, re, json, collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from c35 import *
ent=ps1_entries(); cat=catalog(); E,F=pdb()
out=sys.argv[1]; os.makedirs(out,exist_ok=True)
SF0=bytes.fromhex('568bf18b4e38ff5634c64605015ec3')
res=[]
PM={'CScriptThing':'VCScriptThing@@','long':'J'}
for a,n,lf,src in rows():
    m=re.match(r'^CSpawnedFunc<NScript::(\w+)>::SuspendableProcess$',n)
    m1=re.match(r'^CSpawnedFunc1<NScript::(\w+),(\w+)>::SuspendableProcess$',n)
    if not (m or m1): continue
    T=(m or m1).group(1); P=m1.group(2) if m1 else None
    if m: sym='?SuspendableProcess@?$CSpawnedFunc@V%s@NScript@@@@UAEXXZ'%T; tq='CSpawnedFunc<NScript::%s>'%T
    else: sym='?SuspendableProcess@?$CSpawnedFunc1@V%s@NScript@@%s@@UAEXXZ'%(T,PM[P]); tq='CSpawnedFunc1<NScript::%s,%s>'%(T,P)
    pd=[x for x,s in (('Ego_r.pdb',E),('FableWin.pdb',F)) if sym in s]
    if not pd: res.append((a,'DEFER','no PDB '+sym)); continue
    q=cat[a]; b=bytes.fromhex(q[3]); ln=int(q[2])
    refs,p=dword_refs(int(a,16))
    if len(refs)!=1: res.append((a,'DEFER','%d dword refs'%len(refs))); continue
    vt=refs[0]-0x10
    s0='%08x'%p.u32(vt); s0n=cat.get(s0,[0,'?'])[1]
    exp=tq.replace(',',', ') if P else tq
    if not (s0n.replace(' ','')==(tq+"::`scalardeletingdestructor'").replace(' ','')):
        res.append((a,'DEFER','vtable 0x%08X slot 0 %s is %s, not %s ??_G'%(vt,s0,s0n,tq))); continue
    if m and b!=SF0: res.append((a,'DEFER','body differs from the 15-byte CSpawnedFunc shape')); continue
    hdr=[]
    if m:
        hdr=["// %s::SuspendableProcess() -- the spawned script thread: call the bound `void (T::*)()` member on the stored object, then"%tq,
             "//   set StopRunning",
             "// Leaf: "+sym,
             "// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, 15 bytes: push esi; mov esi,ecx; mov ecx,[esi+38h] (PClass); call [esi+34h] (Func);"%a.upper(),
             "//   mov byte [esi+5],1 (StopRunning); pop esi; ret."]
    else:
        hdr=["// %s::SuspendableProcess() -- the spawned script thread: call the bound `void (T::*)(%s)` member on the stored object"%(tq,P),
             "//   with a copy of the stored argument Par1, then set StopRunning",
             "// Leaf: "+sym,
             "// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %d bytes."%(a.upper(),ln)]
    hdr+=["// Naming wave 35c: slot 4 of vtable 0x%08X (no RTTI), whose slot 0 0x%s is the landed %s::`scalar deleting"%(vt,s0.upper(),tq),
          "//   destructor' (wave 33 / 29); %s %s. The vtable dword is the only reference to 0x%s."%(' + '.join(pd),sym,a.upper()),
          "//   Was a stand-in (old file %s, Leaf %s)."%(src.split('/')[-1],lf or '-'),
          "// Shape: %s's in-class slot 4 body, emitted out of line by an explicit instantiation of the member; T is"%('engine/CSpawnedFunc.h' if m else 'engine/CSpawnedFunc1.h'),
          "//   complete with single inheritance, so Func is the 4-byte member pointer (`mov ecx,PClass ; call Func`)."]
    if m:
        body='''#include "fable_string.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CScriptThing.h"
#include "engine/CScriptBase.h"
#include "engine/CSpawnedFunc.h"
namespace NScript {
class %s : public CScriptBase { };
}

template void CSpawnedFunc<NScript::%s>::SuspendableProcess();
'''%(T,T)
    else:
        pre='#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY\n' if P=='CScriptThing' else ''
        body='''#include "fable_string.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
%s#include "engine/CScriptThing.h"
#include "engine/CScriptBase.h"
#include "engine/CSpawnedFunc1.h"
namespace NScript {
class %s : public CScriptBase { };
}

template void CSpawnedFunc1<NScript::%s, %s>::SuspendableProcess();
'''%(pre,T,T,P)
    new='\n'.join(hdr)+'\n'+body
    mod=tq
    nsrc=os.path.dirname(ent[a]['Source'])+'/'+fname(tq)[:80]+'_SuspendableProcess_'+a+'.cpp'
    ntest=os.path.dirname(ent[a]['TestSource'])+'/'+fname(tq)[:80]+'_SuspendableProcess_'+a+'_test.cpp'
    write(os.path.join(out,'src',nsrc),new)
    res.append((a,'OK',json.dumps(dict(name=tq+'::SuspendableProcess',module=mod,sym=sym,old_src=ent[a]['Source'],new_src=nsrc,old_test=ent[a]['TestSource'],new_test=ntest,vt='%08x'%vt,old_module=ent[a].get('Module',''),old_name=n))))
cnt=collections.Counter(json.loads(r[2])['sym'] for r in res if r[1]=='OK')
res=[(r[0],'DEFER','symbol named by %d rows'%cnt[json.loads(r[2])['sym']]) if r[1]=='OK' and cnt[json.loads(r[2])['sym']]>1 else r for r in res]
open(os.path.join(out,'gen.tsv'),'w').write(''.join('\t'.join(r)+'\n' for r in res))
print(collections.Counter(r[1] for r in res))
for r in res:
    if r[1]=='DEFER': print(r[0],r[2][:200])
