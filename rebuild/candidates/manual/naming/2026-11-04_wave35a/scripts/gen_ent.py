# gen_ent.py <outdir> [addr...]: entity-script GetParentScript / trivial OnPredicateFail / trivial OnPersist as the real members
import sys,os,re,json,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from lib35 import *
ent=ps1_entries(); cat=catalog(); EV=evidence()
outdir=sys.argv[1]; only=set(sys.argv[2:]); os.makedirs(outdir,exist_ok=True)
FORCE_A=os.environ.get('FORCE_ALLOC')=='1'
d=json.load(open(W+'/classify.json'))
BODY={'GetParentScript':('8b4114c3','mov eax,[ecx+14h]; ret','CScriptBase* %s::GetParentScript() const','GetParentScript() const','virtual CScriptBase* GetParentScript() const;'),
      'OnPredicateFail':('c3','ret','void %s::OnPredicateFail()','OnPredicateFail()','virtual void OnPredicateFail();'),
      'OnPersist':('c20400','ret 4','void %s::OnPersist(CPersistContext& context)','OnPersist(CPersistContext&)','virtual void OnPersist(CPersistContext& context);')}
DESC={'GetParentScript':'the entity\'s parent level script (ParentClass, +0x14)',
      'OnPredicateFail':'empty override (the entity does nothing when its predicate fails)',
      'OnPersist':'empty override (the entity persists nothing of its own)'}
res=[]
for r in d:
    if r['kind']!='ent' or r['M'] not in BODY: continue
    a=r['addr']
    if only and a not in only: continue
    O,E,M=r['O'],r['E'],r['M']; qual='NScript::%s::%s'%(O,E)
    hexb,asm,sig,short,decl=BODY[M]
    if cat[a][3].lower()!=hexb: continue    # nontrivial body: another generator
    if len(r['pdb'])!=1: res.append((a,'DEFER','PDB has %d symbols for %s::%s'%(len(r['pdb']),qual,M))); continue
    sym=r['pdb'][0]; acc=access_of(sym)
    e=ent[a]; old=open(ROOT+'/rebuild/src/compiled/'+e['Source'],errors='replace').read()
    ev=EV.get(a)
    if not ev or ev[0]!=r['cn']: res.append((a,'DEFER','no anniversary evidence row with the catalog name')); continue
    h,base,body=hdr_class(O,E)
    path=None
    if h and body is not None and re.search(r'\b%s\s*\('%M,body) and not FORCE_A:
        m=re.search(r'(&|\*)\s*ParentClass\s*;',body)
        if M=='GetParentScript' and not m: res.append((a,'DEFER','header %s: no ParentClass'%h)); continue
        ret = ('&ParentClass' if m.group(1)=='&' else 'ParentClass') if M=='GetParentScript' else None
        defs=defines_for(h)
        if M=='GetParentScript': defn0=(sig%qual)+'\n{\n    return %s;\n}\n'%ret
        elif M=='OnPersist': defn0=(sig%qual).replace('CPersistContext& context','CPersistContext&')+'\n{\n}\n'
        else: defn0=(sig%qual)+'\n{\n}\n'
        tries=[('\n'.join('#define '+x for x in defs)+'\n#include "%s"\n'%h,None)]+[('\n'.join(pl)+'\n',src_) for pl,src_ in prelude_for(h)[:6]]
        tu=None
        for t_,src_ in tries:
            st_,_=qcheck(a,t_+defn0,sym)
            if st_.startswith(('MATCH','RELOCATION')): tu=t_; prel=src_; break
        if tu is None: res.append((a,'DEFER','header %s path does not compile/match with any includer prelude'%h)); continue
        cls='engine/%s.h (shared header: nested class %s, ParentClass %s)'%(O,E,'reference' if ret=='&ParentClass' else 'pointer') if M=='GetParentScript' else 'engine/%s.h (shared header: nested class %s declares %s)'%(O,E,short)
        if h!='engine/%s.h'%O: cls=cls.replace('engine/%s.h'%O,h)
        if prel: cls+='; includes as the landed includer rebuild/src/compiled/%s'%prel.split('rebuild/src/compiled/')[-1]
        path='H'
    else:
        al=[x for x,l in r['alloc'] if l]
        if len(al)!=1: res.append((a,'DEFER','%s; no unique landed Alloc for %s (%s)'%('header lacks the member' if h else 'no shared header',qual,r['alloc']))); continue
        aa=al[0]; asrc=open(ROOT+'/rebuild/src/compiled/'+ent[aa]['Source'],errors='replace').read()
        mm=re.search(r'^//\s*Leaf:\s*(\?Alloc@(\S+?)@@SI\S*)',asrc,re.M)
        if not mm or mm.group(2)!='%s@%s@NScript'%(E,O): res.append((a,'DEFER','Alloc %s leaf not ?Alloc@%s@%s@NScript'%(aa,E,O))); continue
        _,tu=strip_head(asrc)
        tu,cpf=fix_cp(tu)
        n=nested(tu,E)
        if not n: res.append((a,'DEFER','Alloc %s source has no class %s'%(aa,E))); continue
        if M=='GetParentScript':
            m=re.search(r'(\w+)\s*(&|\*)\s*ParentClass\s*;\s*//\s*\+0x14',n[1])
            if not m: res.append((a,'DEFER','Alloc %s model: no ParentClass at +0x14'%aa)); continue
            ret='&ParentClass' if m.group(2)=='&' else 'ParentClass'
        if not re.search(r'\b%s\s*\('%M,n[1]):
            # insert the PDB member declaration at the end of the nested class
            st=re.search(r'\bclass\s+(?:\w+::)?%s\s*(:[^{;]*)?\{'%re.escape(E),tu)
            end=block(tu,st.end()-1)
            ins='%s:\n    %s   // PDB %s (slot %d)\n'%(acc,decl,sym,{'GetParentScript':3,'OnPersist':4,'OnPredicateFail':5}[M])
            # keep the indentation of the closing brace
            ls=tu.rfind('\n',0,end)+1
            tu=tu[:ls]+ins+tu[ls:]
            added=True
        else: added=False
        cls='the class model of the landed Alloc row 0x%s (rebuild/src/compiled/%s)%s'%(aa.upper(),ent[aa]['Source'],'; the PDB member declaration added' if added else '')+('; its local CCountedPointer stand-in replaced by engine/CCountedPointer.h (wave 33 D4)' if cpf else '')
        path='A'
    if M=='GetParentScript': defn=(sig%qual)+'\n{\n    return %s;\n}\n'%ret
    elif M=='OnPersist': defn=(sig%qual).replace('CPersistContext& context','CPersistContext&')+'\n{\n}\n'
    else: defn=(sig%qual)+'\n{\n}\n'
    if path=='A' and not tu.endswith('\n'): tu+='\n'
    oldc=first_comment(old)
    hdr=['// %s::%s -- %s'%(qual,short,DESC[M]),
         '// Leaf: %s'%sym,
         '// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %s bytes: %s.'%(a.upper(),cat[a][2],asm),
         '// Identity (naming wave 35, family ent): %s.'%ev[1].replace('; LANDED row (respell source)',''),
         '//   %s in %s (%s).'%(sym,pdbs(sym),ev[2]),
         '// Was: %s (catalog stand-in %s, Leaf %s).'%(oldc,e['Module'],r['leaf'] or 'none'),
         '// Class: %s; the Alloc body (if any) is not part of this graded body.'%cls]
    new='\n'.join(hdr)+'\n'+tu+('\n' if path=='A' else '')+defn
    dd=os.path.dirname(e['Source']); base=fname(qual)+'_'+M+'_'+a
    nsrc=dd+'/'+base+'.cpp'; ntest=os.path.dirname(e['TestSource'])+'/'+base+'_test.cpp'
    p=os.path.join(outdir,'src',nsrc); os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'w').write(new)
    res.append((a,'OK',json.dumps(dict(qual=qual,name=M,module=qual,sym=sym,path=path,old_src=e['Source'],new_src=nsrc,old_test=e['TestSource'],new_test=ntest,old_module=e['Module'],cat=r['cn']))))
with open(os.path.join(outdir,'gen.tsv'),'w') as f:
    for x in res: f.write('\t'.join(x)+'\n')
print(collections.Counter((x[1],json.loads(x[2])['path'] if x[1]=='OK' else x[2][:40]) for x in res))
