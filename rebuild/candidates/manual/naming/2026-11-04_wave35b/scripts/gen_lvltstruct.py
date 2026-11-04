# gen_lvltstruct.py <outdir>: TSTRUCT level-script rows (Hook / Clear) on engine/CLevelScriptBase.h, members named from Ego_r.pdb - 0x14
import sys,re,json,os
from common35 import *
from lvl import *
out=os.path.abspath(sys.argv[1]); os.makedirs(out,exist_ok=True)
ent=ps1_entries(); cat=catalog(); C=cats(); EV=evidence()
res=[]
def fail(a,why): res.append((a,'DEFER',why))
TY={'bool':('bool',1),'char':('char',1),'long':('long',4),'int':('long',4),'unsigned long':('unsigned long',4),'float':('float',4)}
for a,(addr,k,sym,t,name) in sorted(C.items()):
    if k!='TSTRUCT' or t.endswith('EAEXXZ'): continue
    classes,meth=split_target(name)
    if len(classes)!=1: fail(a,'nested'); continue
    X=classes[0]; old=sym.split('@')[0][1:]
    src=rowsrc(ent[a]['Source'])
    T=datatype(X) or 'CQ_SunnyvaleMasterData'
    tnote='' if datatype(X) else ' (CLevelScriptBase<CQ_SunnyvaleMasterData>, the quest-script default)'
    PLAIN=plain_script(X); MSTART=0x44 if PLAIN else 0x48
    mem=[]; body=[]
    if old=='Hook': what='empty: retail is a lone `ret`'
    elif old=='Clear':
        decl=re.search(r'struct T \{(.*?)\n\};',src,re.S).group(1)
        off=0; fld={}
        for ln in decl.split('\n'):
            ln=ln.strip()
            m=re.match(r'^(char|bool|int)\s+(\w+)(\[(0x[0-9a-fA-F]+|\d+)\])?;$',ln)
            if not m: continue
            n=int(m.group(4),0) if m.group(4) else 1
            fld[m.group(2)]=(off,m.group(1),n); off+={'char':1,'bool':1,'int':4}[m.group(1)]*n
        stm=re.findall(r'this->(\w+) = (\w+);',re.search(r'void T::Clear\(\) \{(.*)\}',src).group(1))
        ok=True; members={}
        for f,v in stm:
            o,ty,n=fld[f]
            em=egor_member_rt(X,o)
            if not em or em[0] not in TY: ok=False; break
            mty,msz=TY[em[0]]
            if msz!={'char':1,'bool':1,'int':4}[ty]: ok=False; break
            members[o]=(em[1],mty,msz)
            val={'false':'false','true':'true','0':'false','1':'true'}.get(v) if mty=='bool' else (v if v not in('false','true') else {'false':'0','true':'1'}[v])
            body.append('    %s = %s;'%(em[1],val))
        if not ok: fail(a,'Clear field not in Ego_r layout of %s at +0x14'%X); continue
        cur=MSTART
        for o in sorted(members):
            nm,mty,msz=members[o]
            if o<cur: ok=False; break
            if o>cur: mem.append('    unsigned char _pad%x[0x%x - 0x%x];'%(cur,o,cur))
            mem.append('    %s %s;%s// +0x%x (Ego_r.pdb +0x%x)'%(mty,nm,' '*max(1,34-len(mty)-len(nm)),o,o+0x14)); cur=o+msz
        if not ok: fail(a,'member below 0x48'); continue
        what='reset the script state: %s'%', '.join(b.strip().rstrip(';') for b in body)
    else: fail(a,'shape %s'%old); continue
    ln=int(cat[a][2]); evf,evs=EV.get(a,('',''))
    slot={'RegisterMain':1,'Main':2,'Init':3}[meth]
    hdr=['// %s() -- %s'%(name,what),'// Leaf: %s'%t,
         '// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %d bytes, thiscall, plain `ret`.'%(a.upper(),ln),
         '// Identity: %s (%s); PDB %s in %s.'%(short_ev(evs),'/'.join(evf.split('/')[-2:]),t,pdbwhere(t)),
         '// Naming wave 35: was landed as the gap-scan stand-in T::%s() on a `struct T` pad model (%s, `%s`); same bytes.'%(old,ent[a]['Source'],sym),
         '// Model: engine/CLevelScriptBase.h%s (Game +0x40, MasterScriptData +0x44); the members this body stores are named from'%tnote,
         '//   Ego_r.pdb %s at +0x14 (the retail script base is 0x14 smaller), the rest is not modelled here.'%X]
    if old=='Hook': hdr[-2]='// Model: engine/CLevelScriptBase.h%s (Game +0x40, MasterScriptData +0x44); %s declared public virtual as the PDB spells it.'%(tnote,meth); hdr.pop()
    if PLAIN:
        b=['#define FABLE_RETAIL_NSCRIPT','#include "engine/CScriptBase.h"','class CGameScriptInterfaceBase;','namespace NScript {','class %s : public CScriptBase {'%X,'public:',
           '    virtual void %s();%s// slot %d'%(meth,' '*max(1,30-len(meth)),slot),'    CGameScriptInterfaceBase* Game;%s// +0x40 (the ctor\'s CGameScriptInterfaceBase&)'%(' '*9)]+mem
        hdr=[h.replace('engine/CLevelScriptBase.h%s (Game +0x40, MasterScriptData +0x44)'%tnote,'engine/CScriptBase.h + Game +0x40 (PDB ctor %s(CGameScriptInterfaceBase&): no master data)'%X) for h in hdr]
    else:
        b=['#define FABLE_RETAIL_NSCRIPT','#include "engine/CLevelScriptBase.h"','namespace NScript {','class %s;'%T,'class %s : public CLevelScriptBase<%s> {'%(X,T),'public:',
       '    virtual void %s();%s// slot %d'%(meth,' '*max(1,30-len(meth)),slot)]+mem
    b+=['};','void %s::%s()'%(X,meth),'{']+body+['}','}']
    newmod='NScript::%s'%X; base=fname(newmod)[:80]+'_'+meth+'_'+a
    nsrc=os.path.dirname(ent[a]['Source'])+'/'+base+'.cpp'; ntest=os.path.dirname(ent[a]['TestSource'])+'/'+base+'_test.cpp'
    p=os.path.join(out,'src',nsrc); os.makedirs(os.path.dirname(p),exist_ok=True)
    open(p,'w').write('\n'.join(hdr+b)+'\n')
    res.append((a,'OK',json.dumps(dict(path=p,leaf=t,module=newmod,new_src=nsrc,new_test=ntest,old_src=ent[a]['Source'],old_test=ent[a]['TestSource'],old_sym=sym,fam='lvltstruct-'+old))))
with open(out+'/gen.tsv','w') as f:
    for r in res: f.write('\t'.join(r)+'\n')
import collections; print(collections.Counter(r[1] for r in res))
for r in res:
    if r[1]!='OK': print(r)
