# gen_minent.py <outdir> <addr...>: TSTRUCT entity rows with no usable landed Alloc TU (Hook / Run) on engine/CEntityScriptBase.h alone
import sys,re,json,os
from common35 import *
out=os.path.abspath(sys.argv[1]); only=set(sys.argv[2:]); os.makedirs(out,exist_ok=True)
ent=ps1_entries(); cat=catalog(); C=cats(); EV=evidence()
SLOTS={}
for l in open(ROOT+'/rebuild/include/engine/CGameScriptInterfaceBase.h'):
    m=re.search(r'virtual \S+ (\w+)\(([^)]*)\)( const)? = 0;\s*// +(\d+) 0x([0-9A-F]+)',l)
    if m: SLOTS[int(m.group(4))]=(m.group(1),m.group(2),m.group(5))
res=[]
for a in sorted(only):
    addr,k,sym,t,name=C[a]
    classes,meth=split_target(name); outer,inner=classes
    old=sym.split('@')[0][1:]; src=rowsrc(ent[a]['Source'])
    inc=[]
    if old=='Hook': body=''; what='empty: retail is a lone `ret`'
    elif old=='Run':
        n=len(re.findall(r'virtual void slot\d+\(\);',src)); fn,args,va=SLOTS[n]
        assert args.replace(' ','')=='constCScriptThing&,bool'
        body='    Game->%s(Me, false);\n'%fn; what='Game->%s(Me, false) (CGameScriptInterfaceBase slot %d, +0x%x, 0x%s)'%(fn,n,n*4,va)
        inc=['// CGameScriptInterfaceBase: the retail interface (vtable slots from engine/CGameScriptInterfaceBase.h).','#include "engine/CGameScriptInterfaceBase.h"']
    else: res.append((a,'DEFER','shape')); continue
    ln=int(cat[a][2]); evf,evs=EV.get(a,('',''))
    hdr=['// %s() -- %s'%(name,what),'// Leaf: %s'%t,
         '// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %d bytes, thiscall, plain `ret`.'%(a.upper(),ln),
         '// Identity: %s (%s); PDB %s in %s.'%(short_ev(evs),'/'.join(evf.split('/')[-2:]),t,pdbwhere(t)),
         '// Naming wave 35: was landed as the gap-scan stand-in T::%s() on a `struct T` pad model (%s, `%s`); same bytes.'%(old,ent[a]['Source'],sym),
         '// Model: engine/CEntityScriptBase.h (vptr, Game +0x04, Me +0x08); the entity has no landed PDB-named Alloc TU to take the',
         '//   class model from, so only the base this body touches is modelled; %s private virtual as the PDB spells it.'%meth]
    b=['#include "engine/CCountedPointer.h"%s// CCountedPointer<T> for CScriptThing::PImp (header contract)'%(' '*21),'#define FABLE_RETAIL_NSCRIPT','#include "engine/CEntityScriptBase.h"']+inc+['namespace NScript {',
       'class %s {%s// the PDB owner script; only its nested entity class is modelled here'%(outer,' '*4),'public:','    class %s;'%inner,'};',
       'class %s::%s : public CEntityScriptBase {'%(outer,inner),'private:','    virtual void %s();%s// PDB %s (slot 2)'%(meth,' '*max(1,30-len(meth)),t),'};',
       'void %s::%s::%s()'%(outer,inner,meth),'{'+('\n'+body.rstrip('\n') if body else ''),'}','}']
    newmod='NScript::%s::%s'%(outer,inner); base=fname(newmod)[:80]+'_'+meth+'_'+a
    nsrc=os.path.dirname(ent[a]['Source'])+'/'+base+'.cpp'; ntest=os.path.dirname(ent[a]['TestSource'])+'/'+base+'_test.cpp'
    p=os.path.join(out,'src',nsrc); os.makedirs(os.path.dirname(p),exist_ok=True)
    open(p,'w').write('\n'.join(hdr+b)+'\n')
    res.append((a,'OK',json.dumps(dict(path=p,leaf=t,module=newmod,new_src=nsrc,new_test=ntest,old_src=ent[a]['Source'],old_test=ent[a]['TestSource'],old_sym=sym,fam='minent-'+old))))
with open(out+'/gen.tsv','w') as f:
    for r in res: f.write('\t'.join(r)+'\n')
import collections; print(collections.Counter(r[1] for r in res))
