# gen_tstruct.py <outdir>: TSTRUCT entity rows (Hook / Clear / Run) on the landed Alloc row's class model
import sys,re,json,os
from common35 import *
from entmodel import model, members
from lvl import egor_member_rt
out=os.path.abspath(sys.argv[1]); os.makedirs(out,exist_ok=True)
ent=ps1_entries(); cat=catalog(); C=cats(); EV=evidence()
SLOTS={}
for l in open(ROOT+'/rebuild/include/engine/CGameScriptInterfaceBase.h'):
    m=re.search(r'virtual \S+ (\w+)\(([^)]*)\)( const)? = 0;\s*// (\d+) 0x([0-9A-F]+)',l)
    if m: SLOTS[int(m.group(4))]=(m.group(1),m.group(2),m.group(5))
res=[]
def fail(a,why): res.append((a,'DEFER',why))
for a,(addr,k,sym,t,name) in sorted(C.items()):
    if k!='TSTRUCT' or not t.endswith('EAEXXZ'): continue
    classes,meth=split_target(name)
    if len(classes)!=2: fail(a,'not nested'); continue
    outer,inner=classes
    old=sym.split('@')[0][1:]
    src=rowsrc(ent[a]['Source'])
    if old=='Hook': body=['']; what='empty: retail is a lone `ret` (the entity overrides %s with nothing)'%meth
    elif old=='Clear':
        decl=re.search(r'struct T \{(.*?)\n\};',src,re.S).group(1)
        off=0; fld={}
        for ln in decl.split('\n'):
            ln=ln.strip()
            m=re.match(r'^(char|bool|int)\s+(\w+)(\[(0x[0-9a-fA-F]+|\d+)\])?;$',ln)
            if not m: continue
            n=int(m.group(4),0) if m.group(4) else 1
            fld[m.group(2)]=(off,m.group(1)); off+={'char':1,'bool':1,'int':4}[m.group(1)]*n
        stm=re.findall(r'this->(\w+) = (\w+);',re.search(r'void T::Clear\(\) \{(.*)\}',src).group(1))
        text,aa,asrc,q=model(inner,outer,meth)
        if text is None: fail(a,q); continue
        mem=members(text,inner)
        if mem is None: fail(a,'Alloc model members not parsed (%s)'%asrc); continue
        body=[]; ok=True
        for f,v in stm:
            o,ty=fld[f]
            if o not in mem: ok=False; break
            nm,mty,n=mem[o]
            if n!=1: ok=False; break
            sz={'char':1,'bool':1,'int':4}[ty]
            msz=1 if mty in('bool','char','unsigned char') else 4
            if sz!=msz: ok=False; break
            if mty=='bool': val={'0':'false','false':'false','1':'true','true':'true'}.get(v)
            else: val=v if v not in('false','true') else {'false':'0','true':'1'}[v]
            if val is None: ok=False; break
            body.append('    %s = %s;'%(nm,val))
        if not ok: fail(a,'Clear field not mapped onto the Alloc model'); continue
        what='resets the entity state: %s'%', '.join(b.strip().rstrip(';') for b in body)
    elif old=='Run':
        n=len(re.findall(r'virtual void slot\d+\(\);',src))
        if n not in SLOTS: fail(a,'slot %d'%n); continue
        fn,args,va=SLOTS[n]
        if args.replace(' ','')!='constCScriptThing&,bool': fail(a,'slot %d args %s'%(n,args)); continue
        body=['    Game->%s(Me, false);'%fn]
        what='Game->%s(Me, false) (CGameScriptInterfaceBase slot %d, +0x%x, 0x%s)'%(fn,n,n*4,va)
    elif old=='Bump':
        off=int(re.search(r'struct Sub \{\s*char pad_0\[(0x[0-9a-fA-F]+)\];\s*int counter;',src).group(1),16)
        if not re.search(r'struct T \{\s*char pad_0\[0x14\];\s*Sub\* sub;',src): fail(a,'Bump: sub not at +0x14'); continue
        em=egor_member_rt(outer,off)
        if not em or em[0]!='long': fail(a,'Bump: no Ego_r long of %s at retail +0x%x'%(outer,off)); continue
        body=['    ++ParentClass->%s;'%em[1]]
        what='counts the entity in: ++ParentClass->%s (%s +0x%x)'%(em[1],outer,off)
        bump=(off,em[1])
    else: fail(a,'shape %s (manual)'%old); continue
    text,aa,asrc,q=model(inner,outer,meth)
    if text is None: fail(a,q); continue
    if old=='Bump':
        om=re.search(r'(class %s : public CLevelScriptBase<\w+> \{\npublic:\n)'%re.escape(outer),text)
        if not om or 'ParentClass; ' not in text.replace('ParentClass;','ParentClass; '): fail(a,'Bump: outer model not found in Alloc TU'); continue
        if not re.search(r'%s\* ParentClass;'%re.escape(outer),text): fail(a,'Bump: ParentClass is not a %s*'%outer); continue
        ins='    unsigned char _members48[0x%x - 0x48];%s// +0x48 (the script\'s members before %s; not modelled here)\n    long %s;%s// +0x%x (Ego_r.pdb %s, retail = Ego_r - 0x14 less the STLport container deltas)\n'%(bump[0],' '*4,bump[1],bump[1],' '*max(1,36-len(bump[1])),bump[0],outer)
        if bump[0]==0x48: ins=ins.split('\n',1)[1]
        text=text[:om.end()]+ins+text[om.end():]
    defn='void %s::%s()\n{\n%s\n}'%(q,meth,'\n'.join(b for b in body if b))
    if body==['']: defn='void %s::%s()\n{\n}'%(q,meth)
    text=text.replace('/*@@DEF@@*/',defn)
    if old=='Run' and 'engine/CGameScriptInterfaceBase.h' not in text and 'engine/CGameScriptInterface.h' not in text:
        text=text.replace('#include "engine/CEntityScriptBase.h"','#include "engine/CEntityScriptBase.h"\n// CGameScriptInterfaceBase: the retail interface (vtable slots from engine/CGameScriptInterfaceBase.h).\n#include "engine/CGameScriptInterfaceBase.h"',1)
        if 'engine/CGameScriptInterfaceBase.h' not in text: fail(a,'no CEntityScriptBase include to anchor'); continue
    ln=int(cat[a][2]); evf,evs=EV.get(a,('',''))
    vt=re.search(r'vtable ([0-9a-f]{8}) slot (\d)',evs)
    hdr=['// %s() -- %s'%(name,what),
         '// Leaf: %s'%t,
         '// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %d bytes, thiscall, plain `ret`.'%(a.upper(),ln),
         '// Identity: %s (%s); PDB %s in %s.'%(short_ev(evs),evf.split('/')[-2]+'/'+evf.split('/')[-1] if '/' in evf else evf,t,pdbwhere(t)),
         '// Naming wave 35: was landed as the gap-scan stand-in T::%s() on a `struct T` pad model (%s, `%s`); same bytes.'%(old,ent[a]['Source'],sym),
         '// Class model: the landed Alloc row 0x%s (rebuild/src/compiled/%s) as wave 33 D1 emits from it: its user-declared'%(aa.upper(),asrc),
         '//   destructor dropped (the entity destructor is implicit), %s declared private virtual as the PDB spells it; the Alloc body is not part of this TU.'%meth]
    newmod='NScript::%s::%s'%(outer,inner)
    base=fname(newmod)[:80]+'_'+meth+'_'+a
    nsrc=os.path.dirname(ent[a]['Source'])+'/'+base+'.cpp'
    ntest=os.path.dirname(ent[a]['TestSource'])+'/'+base+'_test.cpp'
    p=os.path.join(out,'src',nsrc); os.makedirs(os.path.dirname(p),exist_ok=True)
    open(p,'w').write('\n'.join(hdr)+'\n'+text.rstrip('\n')+'\n')
    res.append((a,'OK',json.dumps(dict(path=p,leaf=t,module=newmod,new_src=nsrc,new_test=ntest,old_src=ent[a]['Source'],old_test=ent[a]['TestSource'],old_sym=sym,alloc=aa,fam='tstruct-'+old))))
with open(out+'/gen.tsv','w') as f:
    for r in res: f.write('\t'.join(r)+'\n')
import collections; print(collections.Counter(r[1] for r in res)); 
for r in res:
    if r[1]!='OK': print(r)
