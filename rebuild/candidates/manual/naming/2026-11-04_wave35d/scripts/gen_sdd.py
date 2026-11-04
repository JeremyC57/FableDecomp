# gen_sdd.py <outdir> [addr...]: level-script / entity / CEntityScriptBase ??_G from the landed ctor / Alloc row's TU (wave 33 ctor / esb)
import sys,os,re,json,collections,struct
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from lib35 import *
ent=ps1_entries(); cat=catalog(); EV=evidence(); P=pdbmap()
outdir=sys.argv[1]; only=set(sys.argv[2:]); os.makedirs(outdir,exist_ok=True)
byname={}
for a,q in cat.items(): byname.setdefault(q[1],[]).append(a)
def calls_of(a):
    return ['%08x=%s'%(t,cat.get('%08x'%t,[0,'?'])[1]) for t in call_targets(a)]
OVS={}
if os.path.exists(W+'/ctor/gen.tsv'):
    for _l in open(W+'/ctor/gen.tsv'):
        _q=_l.rstrip('\n').split('\t')
        if _q[1]=='OK': OVS[_q[0]]=json.loads(_q[2])['new_src']
def srcof(a): return OVS.get(a,ent[a]['Source'])
def leafof(a):
    s=open((W+'/ctor/src/' if a in OVS else ROOT+'/rebuild/src/compiled/')+srcof(a),errors='replace').read()
    m=re.search(r'^//\s*Leaf:\s*(\S+)',s,re.M); return (m.group(1) if m else None),s
res=[]
for l in list(open(W+'/ROWS.tsv'))[1:]:
    a,cn,leaf,src=l.rstrip('\n').split('\t')
    if only and a not in only: continue
    m=re.match(r"^NScript::(\w+)::(?:(\w+)::)?`scalar deleting destructor'$",cn)
    if cn=="CEntityScriptBase::`scalar deleting destructor'": O=E=None; kind='base'
    elif m: O,E=m.groups(); kind='lvl' if E is None else 'ent'
    else: continue
    q={'base':'CEntityScriptBase','lvl':'%s@NScript'%O,'ent':'%s@%s@NScript'%(E,O)}[kind]
    qual={'base':'CEntityScriptBase','lvl':'NScript::%s'%O,'ent':'NScript::%s::%s'%(O,E)}[kind]
    last={'base':'CEntityScriptBase','lvl':O,'ent':E}[kind]
    gsym='??_G%s@@UAEPAXI@Z'%q
    pd=['%s %s'%(pdbs(s),s) for s in ('??_G%s@@UAEPAXI@Z'%q,'??_E%s@@UAEPAXI@Z'%q,'??1%s@@UAE@XZ'%q) if s in P]
    if not pd: res.append((a,'DEFER','no PDB ??_G / ??_E / ??1 for %s'%qual)); continue
    # storer row: level ctor / entity Alloc / (base) the CClickableDoor Alloc 0x00CDEE10 (wave 33 esb precedent)
    if kind=='lvl': st=[x for x in byname.get('NScript::%s::%s'%(O,O),[]) if x in ent]; want=r'^\?\?0%s@NScript@@[QIA]AE'%O; what='constructor'
    elif kind=='ent': st=[x for x in byname.get('NScript::%s::%s::Alloc'%(O,E),[]) if x in ent]; want=r'^\?Alloc@%s@%s@NScript@@SI'%(E,O); what='Alloc'
    else: st=['00cdee10']; want=r'^\?Alloc@CClickableDoor@CPersonalScript_GlobalThingsScript@NScript@@SI'; what='Alloc'
    if len(st)!=1: res.append((a,'DEFER','no unique landed %s row for %s (%s)'%(what,qual,st))); continue
    st=st[0]; lf,ssrc=leafof(st); lfnote=''
    if lf and '@@' not in lf:
        cs=[x for x in P if x.startswith(lf+'@') and re.match(want,x)]
        if len(cs)==1: lfnote=' (the row\'s Leaf %s, completed from the PDB name set)'%lf; lf=cs[0]
    if not lf or not re.match(want,lf): res.append((a,'DEFER','%s row 0x%s Leaf %s is not the PDB %s of %s'%(what,st,lf,what,qual))); continue
    if lf not in P: res.append((a,'DEFER','%s row Leaf %s not in the PDB name sets'%(what,lf))); continue
    _,body=strip_head(ssrc)
    variants=[]
    b0=body; n0='its own source unchanged'
    if '#include "engine/CLevelScriptBase.h"' in b0 and 'FABLE_RETAIL_CLEVELSCRIPTBASE' not in b0:
        b0=b0.replace('#include "engine/CLevelScriptBase.h"','#define FABLE_RETAIL_CLEVELSCRIPTBASE   // retail\'s implicit ~CLevelScriptBase<T> (chunk 713 opt-in): the ??_G calls ~CScriptBase 0x00CBD510\n#include "engine/CLevelScriptBase.h"',1)
        n0='its own source with the FABLE_RETAIL_CLEVELSCRIPTBASE opt-in added'
    b0c,cpf=fix_cp(b0)
    if cpf: n0+='; its local CCountedPointer stand-in replaced by engine/CCountedPointer.h (wave 33 D4)'
    variants.append((b0c,n0))
    if kind!='base':
        pat=re.compile(r'^[ \t]*(virtual )?~%s\(\)( throw\(\))?;[^\n]*\n'%re.escape(last),re.M)
        if len(pat.findall(b0c))==1 and not re.search(r'%s::~%s\s*\('%(re.escape(last),re.escape(last)),b0c):
            variants.append((pat.sub('',b0c),n0+'; its user-declared destructor dropped (retail expands the destructor into the ??_G)'))
    if os.environ.get('DROP_FIRST')=='1': variants=variants[::-1]   # wave 35d: retail's ??_G calls the base dtor directly -> the derived dtor is implicit
    fl=ent[st].get('CompilerFlags','')
    extra=[f for f in fl.split() if f not in ('/O2','/Oy','/W3')]
    done=None; tried=[]
    for v,note in variants:
        s_,_=qcheck(a,v,gsym,extra)
        tried.append(s_)
        if s_.startswith(('MATCH','RELOCATION')): done=(v,note); break
    if not done: res.append((a,'DEFER','%s TU of 0x%s does not reproduce the ??_G: %s'%(what,st,' / '.join(tried)))); continue
    v,note=done; ev=EV[a]
    hdr=["// %s::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set"%qual,
         "// Leaf: %s"%gsym,
         "// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %s bytes, thiscall, `ret 4`; calls %s."%(a.upper(),cat[a][2],' ; '.join(calls_of(a))),
         "// Identity (naming wave 35, family sdd): %s."%ev[1].replace('; LANDED row (respell source)',''),
         "//   %s (%s); PDB: %s."%(gsym,ev[2],'; '.join(pd)),
         "//   Was: %s (catalog stand-in %s, Leaf %s)."%(first_comment(open(ROOT+'/rebuild/src/compiled/'+ent[a]['Source'],errors='replace').read()),ent[a]['Module'],leaf or 'none'),
         "// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 %s): the class model and"%('ctor' if kind=='lvl' else 'esb'),
         "//   the PDB %s %s%s of the landed row 0x%s (rebuild/src/compiled/%s), %s. The %s body is not part of this graded body."%(what,lf,lfnote,st.upper(),srcof(st),note,what)]
    new='\n'.join(hdr)+'\n'+v
    dd=os.path.dirname(ent[a]['Source']); base=fname(qual)+'_scalardeletingdestructor_'+a
    nsrc=dd+'/'+base+'.cpp'; ntest=os.path.dirname(ent[a]['TestSource'])+'/'+base+'_test.cpp'
    p=os.path.join(outdir,'src',nsrc); os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'w').write(new)
    res.append((a,'OK',json.dumps(dict(qual=qual,name="`scalar deleting destructor'",module=qual,sym=gsym,path=kind,storer=st,flags=fl,old_src=ent[a]['Source'],new_src=nsrc,old_test=ent[a]['TestSource'],new_test=ntest,old_module=ent[a]['Module'],cat=cn))))
with open(os.path.join(outdir,'gen.tsv'),'w') as f:
    for x in res: f.write('\t'.join(x)+'\n')
print(collections.Counter((x[1],json.loads(x[2])['path'] if x[1]=='OK' else '') for x in res))
