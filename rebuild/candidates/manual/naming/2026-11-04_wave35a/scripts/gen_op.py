# gen_op.py <outdir> [addr...]: OnPersist bodies of 1-2 CPersistContext::Transfer<bool|long> calls (entity and level scripts)
import sys,os,re,json,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from lib35 import *
ent=ps1_entries(); cat=catalog(); EV=evidence(); P=pdbmap()
outdir=sys.argv[1]; only=set(sys.argv[2:]); os.makedirs(outdir,exist_ok=True)
byname={}
for a,q in cat.items(): byname.setdefault(q[1],[]).append(a)
TT={'004045c0':('bool','false','??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z'),'00410be0':('long','0L','??$Transfer@J@CPersistContext@@QAEXPBDAAJABJ@Z')}
def member_type(body,K):
    m=re.search(r'^[ \t]*((?:unsigned\s+|signed\s+)?[\w:<>]+(?:\s*[*&])?)\s+%s\s*;'%re.escape(K),body,re.M)
    if m: return m.group(1).strip()
    m=re.search(r'^[ \t]*(bool|long)\s+((?:\w+\s*,\s*)*?)%s\s*[,;]'%re.escape(K),body,re.M)   # comma list
    if m: return m.group(1)
    m=re.search(r'^[ \t]*(bool|long)\s+[\w\s,]*\b%s\b[\w\s,]*;'%re.escape(K),body,re.M|re.S)
    return m.group(1) if m else None
SZ={'bool':1,'long':4}
def patch_members(tu,cls,adds):
    """adds: [(off,type,name)] -- insert members the current byte-matching source places at these offsets into a class whose model has
    no member there: split an `unsigned char X[A - B];` pad that covers them, or (no data members after +0x48) open the class at +0x48."""
    st=re.search(r'\bclass\s+(?:\w+::)?%s\s*(:[^{;]*)?\{'%re.escape(cls),tu)
    if not st: return None
    end=block(tu,st.end()-1); body=tu[st.end():end]
    pads=list(re.finditer(r'^([ \t]*)unsigned char\s+(\w+)\[(0x[0-9A-Fa-f]+)\s*-\s*(0x[0-9A-Fa-f]+)\];[^\n]*\n',body,re.M))
    adds=sorted(adds)
    def lines(ind,lo,hi,tag):
        out=[]; cur=lo
        for off,ty,nm in adds:
            if off>cur: out.append('%sunsigned char _pad%03x[0x%X - 0x%X];   // members not touched here\n'%(ind,cur,off,cur))
            out.append('%s%s %s;   // +0x%x (persisted under this name; offset from the landed byte-matching OnPersist)\n'%(ind,ty,nm,off)); cur=off+SZ[ty]
        if hi is not None and hi>cur: out.append('%sunsigned char _pad%03x[0x%X - 0x%X];\n'%(ind,cur,hi,cur))
        return ''.join(out)
    for pm in pads:
        hi=int(pm.group(3),16); lo=int(pm.group(4),16)
        if all(lo<=o and o+SZ[t]<=hi for o,t,n in adds):
            nb=body[:pm.start()]+lines(pm.group(1),lo,hi,pm.group(2))+body[pm.end():]
            return tu[:st.end()]+nb+tu[end:]
    data=[l for l in body.split('\n') if re.match(r'^\s*[\w:<>]+[\s*&]+\w+(\[[^\]]*\])?\s*;',l) and '(' not in l]
    # append after the last data member when its offset comment and size are known and no `new` of the class is in the TU
    TS={'CTimer':4,'CScriptThing':0xc,'bool':1,'long':4}
    if data and not re.search(r'\bnew\s+%s\b'%re.escape(cls),tu):
        lm=re.match(r'^\s*(\w+)\s+\w+\s*;\s*//\s*\+0x([0-9a-fA-F]+)',data[-1])
        if lm and lm.group(1) in TS:
            lend=int(lm.group(2),16)+TS[lm.group(1)]
            if all(o>=lend for o,t,n in adds):
                li=body.index(data[-1])+len(data[-1])+1
                nb=body[:li]+lines('    ',lend,None,'')+body[li:]
                return tu[:st.end()]+nb+tu[end:]
    if not data and not pads:
        ls=tu.rfind('\n',0,end)+1
        return tu[:ls]+'public:\n'+lines('    ',0x48,None,'')+tu[ls:]
    return None
def model_tu(a,O,E,lvl):
    """-> (tu text, class body, how) using the shared header if it declares the class+members, else the landed Alloc/ctor row's TU"""
    out=[]
    for h in ('engine/%s.h'%O,'engine/NScript_%s.h'%O):
        if os.path.exists(ROOT+'/rebuild/include/'+h):
            t=open(ROOT+'/rebuild/include/'+h).read()
            n=nested(t,O if lvl else E)
            if n: out.append(('H',h,n[1]))
    al=byname.get('NScript::%s::%s'%(O,O) if lvl else 'NScript::%s::%s::Alloc'%(O,E),[])
    al=[x for x in al if x in ent]
    if len(al)==1:
        s=open(ROOT+'/rebuild/src/compiled/'+ent[al[0]]['Source'],errors='replace').read()
        _,tu=strip_head(s)
        n=nested(tu,O if lvl else E)
        if n: out.append(('A',al[0],tu))
    return out
res=[]
for l in list(open(W+'/ROWS.tsv'))[1:]:
    a,cn,leaf,src=l.rstrip('\n').split('\t')
    m=re.match(r'^NScript::(\w+)::(?:(\w+)::)?OnPersist$',cn)
    if not m or int(cat[a][2])<=3 or (only and a not in only): continue
    O,E=m.groups(); lvl=E is None
    qual='NScript::%s'%O+('' if lvl else '::'+E)
    sym=[s for s in P if s.startswith('?OnPersist@%s@NScript@@'%(O if lvl else E+'@'+O))]
    if len(sym)!=1: res.append((a,'DEFER','PDB OnPersist symbols: %s'%sym)); continue
    sym=sym[0]; acc=access_of(sym)
    e=ent[a]; old=open(ROOT+'/rebuild/src/compiled/'+e['Source'],errors='replace').read()
    calls=re.findall(r'Transfer\("(\w+)"',old)
    tgt=set(x.lower() for x in re.findall(r'0x(00[0-9A-Fa-f]{6})',old)) & set(TT)
    if not calls or len(tgt)!=1: res.append((a,'DEFER','old source: keys %s targets %s'%(calls,tgt))); continue
    ty,dv,tsym=TT[tgt.pop()]
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    offs=[]
    for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes.fromhex(cat[a][3]),int(a,16)):
        mm=re.match(r'(?:add e[cs][xi], |lea \w+, \[e[cs][xi] \+ )(0x[0-9a-f]+)',i.mnemonic+' '+i.op_str) if i.mnemonic in ('add','lea') else None
        if mm: offs.append(int(mm.group(1),16))
    ev=EV[a]
    done=None; why=[]
    for how,ref,text in model_tu(a,O,E,lvl):
        body=nested(text,O if lvl else E)[1] if how=='A' else text
        mt=[member_type(body,K) for K in calls]
        patched=''
        if how=='A' and all(t is None for t in mt) and offs and len(offs)==len(calls):
            t2=patch_members(text,O if lvl else E,[(o,ty,K) for o,K in zip(offs,calls)])
            if t2: text=t2; body=nested(text,O if lvl else E)[1]; mt=[member_type(body,K) for K in calls]; patched='; members %s added at the offsets of the landed byte-matching source (the persisted keys are their names)'%', '.join('%s +0x%x'%(K,o) for o,K in zip(offs,calls))
        if any(t!=ty for t in mt): why.append('%s %s: member types %s (want %s)'%(how,ref,mt,ty)); continue
        decl_present=re.search(r'\bOnPersist\s*\(',body)
        if how=='H':
            if not decl_present: why.append('H %s: no OnPersist declaration'%ref); continue
            pl=[('\n'.join('#define '+x for x in defines_for(ref))+'\n#include "%s"\n'%ref,None)]+[('\n'.join(p_)+'\n',s_) for p_,s_ in prelude_for(ref)[:6]]
            cands=[(p_+'#include "engine/CPersistContext.h"\n',s_) for p_,s_ in pl]
        else:
            tu=text
            if not decl_present:
                st=re.search(r'\bclass\s+(?:\w+::)?%s\s*(:[^{;]*)?\{'%re.escape(O if lvl else E),tu)
                end=block(tu,st.end()-1); ls=tu.rfind('\n',0,end)+1
                tu=tu[:ls]+'%s:\n    virtual void OnPersist(CPersistContext& context);   // PDB %s (slot 4)\n'%(acc,sym)+tu[ls:]
            if re.search(r'\bclass\s+CPersistContext\s*\{',tu): why.append('A %s: local CPersistContext model'%ref); continue
            tu,cpf=fix_cp(tu)
            cands=[('#include "engine/CPersistContext.h"\n'+tu+('\n' if not tu.endswith('\n') else ''),None)]
        defn='void %s::OnPersist(CPersistContext& context)\n{\n'%qual+''.join('    context.Transfer("%s", %s, %s);\n'%(K,K,dv) for K in calls)+'}\n'
        for c,s_ in cands:
            st_,_=qcheck(a,c+'\n'+defn,sym)
            if st_.startswith(('MATCH','RELOCATION')): done=(how,ref,c,s_,decl_present,patched); break
            why.append('%s %s: %s'%(how,ref,st_))
        if done: break
    if not done: res.append((a,'DEFER','; '.join(why) or 'no class model (header / landed Alloc or ctor row)')); continue
    how,ref,c,s_,dp,patched=done
    if how=='H': cls='%s (shared header: %s declares %s and OnPersist)%s'%(ref,O if lvl else E,', '.join('%s %s'%(ty,K) for K in calls),('; includes as the landed includer rebuild/src/compiled/%s'%s_.split('rebuild/src/compiled/')[-1]) if s_ else '')
    else: cls='the class model of the landed %s row 0x%s (rebuild/src/compiled/%s)%s; its %s'%('constructor' if lvl else 'Alloc',ref.upper(),ent[ref]['Source'],'' if dp else '; the PDB OnPersist declaration added',', '.join('%s %s'%(ty,K) for K in calls))+patched
    hdr=['// %s::OnPersist(CPersistContext& context) -- saves/loads %s by name (default %s)'%(qual,' and '.join(calls),dv),
         '// Leaf: %s'%sym,
         '// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %s bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<%s> 0x%s (%s, already bound).'%(a.upper(),cat[a][2],ty,'004045C0' if ty=='bool' else '00410BE0',tsym),
         '// Identity (naming wave 35, family op): %s.'%ev[1].replace('; LANDED row (respell source)',''),
         '//   %s in %s (%s). The persisted keys are the PDB member names.'%(sym,pdbs(sym),ev[2]),
         '// Was: %s (catalog stand-in %s).'%(first_comment(old),e['Module']),
         '// Class: %s. CPersistContext: engine/CPersistContext.h. Only this member is graded.'%cls]
    new='\n'.join(hdr)+'\n'+c+'\n'+defn
    dd=os.path.dirname(e['Source']); base=fname(qual)+'_OnPersist_'+a
    nsrc=dd+'/'+base+'.cpp'; ntest=os.path.dirname(e['TestSource'])+'/'+base+'_test.cpp'
    p=os.path.join(outdir,'src',nsrc); os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'w').write(new)
    res.append((a,'OK',json.dumps(dict(qual=qual,name='OnPersist',module=qual,sym=sym,path=how,old_src=e['Source'],new_src=nsrc,old_test=e['TestSource'],new_test=ntest,old_module=e['Module'],cat=cn))))
with open(os.path.join(outdir,'gen.tsv'),'w') as f:
    for x in res: f.write('\t'.join(x)+'\n')
print(collections.Counter((x[1],json.loads(x[2])['path'] if x[1]=='OK' else '') for x in res))
