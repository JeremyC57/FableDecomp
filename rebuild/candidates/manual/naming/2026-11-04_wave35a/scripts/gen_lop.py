# gen_lop.py <outdir>: level-script trivial OnPersist (ret 4) as the real member, in the landed ctor row's TU or the shared header
import sys,os,re,json,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from lib35 import *
ent=ps1_entries(); cat=catalog(); EV=evidence(); P=pdbmap()
outdir=sys.argv[1]; os.makedirs(outdir,exist_ok=True)
byname={}
for a,q in cat.items(): byname.setdefault(q[1],[]).append(a)
res=[]
for l in list(open(W+'/ROWS.tsv'))[1:]:
    a,cn,leaf,src=l.rstrip('\n').split('\t')
    m=re.match(r'^NScript::(\w+)::OnPersist$',cn)
    if not m or cat[a][3]!='c20400': continue
    O=m.group(1); qual='NScript::'+O
    sym='?OnPersist@%s@NScript@@UAEXAAVCPersistContext@@@Z'%O
    if sym not in P: res.append((a,'DEFER','PDB lacks '+sym)); continue
    defn='void %s::OnPersist(CPersistContext&)\n{\n}\n'%qual
    cands=[]
    h='engine/%s.h'%O
    if os.path.exists(ROOT+'/rebuild/include/'+h):
        t=open(ROOT+'/rebuild/include/'+h).read(); n=nested(t,O)
        if n and re.search(r'\bOnPersist\s*\(',n[1]):
            for pl,s_ in prelude_for(h)[:4]: cands.append(('the shared header %s (includes as the landed includer rebuild/src/compiled/%s)'%(h,s_.split('rebuild/src/compiled/')[-1]),'\n'.join(pl)+'\n'))
    st=[x for x in byname.get('%s::%s'%(qual,O),[]) if x in ent]
    if len(st)==1:
        s=open(ROOT+'/rebuild/src/compiled/'+ent[st[0]]['Source'],errors='replace').read()
        lf=re.search(r'^//\s*Leaf:\s*(\S+)',s,re.M)
        if lf and lf.group(1).startswith('??0%s@NScript@@'%O) or (lf and lf.group(1)=='??0'+O):
            tu=strip_head(s)[1]; n=nested(tu,O)
            if n:
                if not re.search(r'\bOnPersist\s*\(',n[1]):
                    x=re.search(r'\bclass\s+(?:\w+::)?%s\s*(:[^{;]*)?\{'%re.escape(O),tu); end=block(tu,x.end()-1); ls=tu.rfind('\n',0,end)+1
                    tu=tu[:ls]+'public:\n    virtual void OnPersist(CPersistContext& context);   // PDB %s (slot 4)\n'%sym+tu[ls:]
                cands.append(('the class model of the landed constructor row 0x%s (rebuild/src/compiled/%s); the PDB OnPersist declaration added where missing'%(st[0].upper(),ent[st[0]]['Source']),tu+('\n' if not tu.endswith('\n') else '')))
    done=None; why=[]
    for d_,t_ in cands:
        s_,_=qcheck(a,t_+'\n'+defn,sym)
        if s_.startswith(('MATCH','RELOCATION')): done=(d_,t_); break
        why.append(s_)
    if not done: res.append((a,'DEFER','no class model reproduces it (%s)'%(' / '.join(why) or 'no header declaring OnPersist, no landed PDB-named ctor row'))); continue
    d_,t_=done; ev=EV[a]; e=ent[a]
    hdr=['// %s::OnPersist(CPersistContext&) -- empty override (the level script persists nothing of its own)'%qual,'// Leaf: %s'%sym,
         '// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, 3 bytes: ret 4.'%a.upper(),
         '// Identity (naming wave 35, family lop): %s.'%ev[1].replace('; LANDED row (respell source)',''),
         '//   %s in %s (%s).'%(sym,pdbs(sym),ev[2]),
         '// Was: %s (catalog stand-in %s).'%(first_comment(open(ROOT+'/rebuild/src/compiled/'+e['Source'],errors='replace').read()),e['Module']),
         '// Class: %s; only this member is graded.'%d_]
    new='\n'.join(hdr)+'\n'+t_+'\n'+defn
    dd=os.path.dirname(e['Source']); base=fname(qual)+'_OnPersist_'+a
    nsrc=dd+'/'+base+'.cpp'; ntest=os.path.dirname(e['TestSource'])+'/'+base+'_test.cpp'
    p=os.path.join(outdir,'src',nsrc); os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'w').write(new)
    res.append((a,'OK',json.dumps(dict(qual=qual,name='OnPersist',module=qual,sym=sym,path='lop',old_src=e['Source'],new_src=nsrc,old_test=e['TestSource'],new_test=ntest,old_module=e['Module'],cat=cn))))
with open(os.path.join(outdir,'gen.tsv'),'w') as f:
    for x in res: f.write('\t'.join(x)+'\n')
print(collections.Counter(x[1] for x in res)); [print(x) for x in res if x[1]=='DEFER']
