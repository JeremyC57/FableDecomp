# gen_entctor.py <outdir> [addr...]: entity-script constructors I(O& parent, CScriptDataBase* data, const CScriptThing& me): the row's own model,
#   its parent stand-in renamed to the PDB outer O (moved into NScript), the class nested as O::I, the PDB parameter types taken
import sys,re,json,os
from common35 import *
from csrc import *
from requal import enclosing_ns, pub_bases, Fail
out=os.path.abspath(sys.argv[1]); only=set(sys.argv[2:]); os.makedirs(out,exist_ok=True)
ent=ps1_entries(); cat=catalog(); C=cats(); EV=evidence()
res=[]
def split_params(p): return [x.strip() for x in re.split(r',(?![^<]*>)',p.strip())] if p.strip() else []
def pname(p):
    m=re.match(r'^(.*?)(\w+)\s*$',p); return (m.group(1).strip(),m.group(2))
for a,(addr,k,sym,t,name) in sorted(C.items()):
    if k!='CTOR' or not t.endswith('QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z'): continue
    if only and a not in only: continue
    classes,meth=split_target(name); O,I=classes
    s=rowsrc(ent[a]['Source'])
    try:
        m=re.match(r'^\?\?0(\w+)@@QAE@PAU(\w+)@@(PAX|J|PAU\w+@@)AB[UV](\w+)@@@Z$',sym)
        if not m: raise Fail('ctor symbol shape %s'%sym)
        D,P,_,Q=m.groups()
        if Q!='CScriptThing': raise Fail('third parameter type %s (not CScriptThing)'%Q)
        if enclosing_ns(s,find_class(s,D)['start']): raise Fail('class in a namespace')
        if D!=I and re.search(r'\b%s\b'%re.escape(I),strip_comments(s)): raise Fail('both %s and %s spelled'%(D,I))
        # 1 parent stand-in -> NScript::O
        if P!=O:
            if re.search(r'\b%s\b'%re.escape(O),strip_comments(s)): raise Fail('both %s and %s spelled'%(P,O))
            s=re.sub(r'\b%s\b'%re.escape(P),O,s)
        od=find_class(s,O)
        if not od or enclosing_ns(s,od['start']): raise Fail('outer stand-in %s not a global class'%O)
        ob=s[od['bo']+1:od['bc']]
        newo='namespace NScript {\nclass %s%s {%s%s%spublic:\n    class %s;%s// the entity class below (naming wave 35)\n};\n}\nusing NScript::%s;'%(O,pub_bases(od['bases']),'\npublic:' if od['key']=='struct' else '',ob,'' if ob.endswith('\n') else '\n',I,' '*max(1,26-len(I)),O)
        s=s[:od['start']]+newo+s[od['end']:]
        s=re.sub(r'(?m)^([ \t]*)(?:struct|class)\s+%s\s*;'%re.escape(O),r'\1namespace NScript { class %s; } using NScript::%s;'%(O,O),s)
        # 2 rename D -> I
        if D!=I:
            s=re.sub(r'\b%s\b'%re.escape(D),I,s)
        d=find_class(s,I)
        if d['start']<s.find('class %s;'%I): raise Fail('entity class before its outer')
        body=s[d['bo']+1:d['bc']]
        dm=list(re.finditer(r'(?m)^([ \t]*)%s\s*\(([^)]*)\)\s*;'%re.escape(I),body))
        if len(dm)!=1: raise Fail('ctor declaration found %d times'%len(dm))
        body=body[:dm[0].start()]+dm[0].group(1)+'%s(%s& parent, CScriptDataBase* data, const CScriptThing& me);'%(I,O)+body[dm[0].end():]
        newdecl='namespace NScript {\nclass %s::%s%s {%s%s};\n}'%(O,I,pub_bases(d['bases']),'\npublic:' if d['key']=='struct' else '',body)
        s=s[:d['start']]+newdecl+s[d['end']:]
        # 3 definition
        hm=[x for x in re.finditer(r'(?m)(?<![\w:])%s\s*::\s*%s\s*\(([^)]*)\)'%(re.escape(I),re.escape(I)),s) if '//' not in s[s.rfind('\n',0,x.start())+1:x.start()]]
        if len(hm)!=1: raise Fail('ctor definition found %d times'%len(hm))
        ps=split_params(hm[0].group(1))
        if len(ps)!=3: raise Fail('%d params'%len(ps))
        (t1,n1),(t2,n2),(t3,n3)=[pname(p) for p in ps]
        rest=s[hm[0].end():]
        bo=rest.find('{'); bc=match_brace(rest,bo)
        seg=rest[:bc+1]
        seg=re.sub(r'\b%s\b'%re.escape(n1),'@@1',seg); seg=re.sub(r'\b%s\b'%re.escape(n2),'@@2',seg); seg=re.sub(r'\b%s\b'%re.escape(n3),'@@3',seg)
        seg=seg.replace('@@1','(&parent)').replace('@@2','((%s)data)'%t2 if t2.replace(' ','')!='CScriptDataBase*' else 'data').replace('@@3','me')
        seg=re.sub(r'\(&parent\)->','parent.',seg)
        s=s[:hm[0].start()]+'NScript::%s::%s::%s(%s& parent, CScriptDataBase* data, const CScriptThing& me)'%(O,I,I,O)+seg+rest[bc+1:]
        s='class CScriptDataBase;\n'+s
    except Fail as e:
        res.append((a,'DEFER',str(e))); continue
    hdr=rowsrc(ent[a]['Source']).split('\n'); j=0
    while j<len(hdr) and hdr[j].startswith('//'): j+=1
    old_hdr=[h for h in hdr[:j] if not h.startswith('// Leaf:')]
    l1=old_hdr[0] if old_hdr else ''
    desc=l1.split(' -- ',1)[1] if ' -- ' in l1 else re.sub(r'^//\s*','',l1)
    evf,evs=EV.get(a,('',''))
    nh=['// %s(%s& parent, CScriptDataBase* data, const CScriptThing& me) -- %s'%(name,O,desc),'// Leaf: %s'%t]+old_hdr[1:]+[
        '// Naming wave 35: identity %s (%s); PDB %s in %s.'%(short_ev(evs),'/'.join(evf.split('/')[-2:]),t,pdbwhere(t)),
        '//   Was `%s`: the parent stand-in %s is the PDB outer NScript::%s and the class is nested in it with the PDB'%(sym,P,O),
        '//   parameter types (the parent by reference, as the PDB spells it; same pointer ABI); same bytes.']
    bl=s.split('\n'); k2=1
    while k2<len(bl) and bl[k2].startswith('//'): k2+=1
    final='\n'.join(nh)+'\n'+'\n'.join(bl[:1]+bl[k2:])
    newmod='NScript::%s::%s'%(O,I); base=fname(newmod)[:80]+'_'+I[:48]+'_'+a
    nsrc=os.path.dirname(ent[a]['Source'])+'/'+base+'.cpp'; ntest=os.path.dirname(ent[a]['TestSource'])+'/'+base+'_test.cpp'
    p=os.path.join(out,'src',nsrc); os.makedirs(os.path.dirname(p),exist_ok=True)
    open(p,'w').write(final)
    res.append((a,'OK',json.dumps(dict(path=p,leaf=t,module=newmod,new_src=nsrc,new_test=ntest,old_src=ent[a]['Source'],old_test=ent[a]['TestSource'],old_sym=sym,fam='entctor'))))
with open(out+'/gen.tsv','w') as f:
    for r in res: f.write('\t'.join(r)+'\n')
import collections; print(collections.Counter(r[1] for r in res))
for r in res:
    if r[1]!='OK': print(r)
