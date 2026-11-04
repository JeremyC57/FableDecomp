# gen_lvlctor.py <outdir> [addr...]: level-script constructors X(CScriptDataBase*, CGameScriptInterfaceBase&): the row's own layout model kept,
#   the class renamed/requalified, the PDB parameter types taken and handed to the row's argument-carrying base unchanged (casts only)
import sys,re,json,os
from common35 import *
from csrc import *
from requal import enclosing_ns, pub_bases, Fail
out=os.path.abspath(sys.argv[1]); only=set(sys.argv[2:]); os.makedirs(out,exist_ok=True)
ent=ps1_entries(); cat=catalog(); C=cats(); EV=evidence(); E,F=pdb()
SIG='CScriptDataBase* data, CGameScriptInterfaceBase& game'
res=[]
def split_params(p):
    p=p.strip()
    if not p: return []
    return [x.strip() for x in re.split(r',(?![^<]*>)',p)]
def pname(p):
    m=re.match(r'^(.*?)(\w+)\s*$',p); return (m.group(1).strip(),m.group(2)) if m else (p,'')
for a,(addr,k,sym,t,name) in sorted(C.items()):
    if k!='CTOR' or not t.endswith('QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z') or t.count('@')!=6+0 and False: pass
    if k!='CTOR' or not t.endswith('@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z'): continue
    if only and a not in only: continue
    classes,meth=split_target(name)
    if len(classes)!=1: continue
    X=classes[0]
    s=rowsrc(ent[a]['Source'])
    try:
        D=sym[3:].split('@')[0]
        d=find_class(s,D)
        if not d: raise Fail('class %s not found'%D)
        ens=enclosing_ns(s,d['start'])
        if ens not in ([],['NScript']): raise Fail('ns %s'%ens)
        body=s[d['bo']+1:d['bc']]
        dm=list(re.finditer(r'(?m)^([ \t]*)%s\s*\(([^)]*)\)\s*;'%re.escape(D),body))
        if len(dm)!=1: raise Fail('ctor declaration found %d times'%len(dm))
        body=body[:dm[0].start()]+dm[0].group(1)+'%s(%s);'%(X,SIG)+body[dm[0].end():]
        s=s[:d['bo']+1]+body+s[d['bc']:]
        # definition
        hm=list(re.finditer(r'(?m)(?<![\w:])(?:NScript\s*::\s*)?%s\s*::\s*%s\s*\(([^)]*)\)\s*:\s*'%(re.escape(D),re.escape(D)),s))
        if len(hm)!=1: raise Fail('ctor definition found %d times'%len(hm))
        ps=split_params(hm[0].group(1))
        if len(ps)!=2: raise Fail('ctor takes %d params'%len(ps))
        (t1,n1),(t2,n2)=pname(ps[0]),pname(ps[1])
        im=re.match(r'(\w+)\s*\(',s[hm[0].end():])
        if not im: raise Fail('first initializer not a base call')
        bo=hm[0].end()+im.end()-1; bc=match_brace_paren=None
        dep=0
        for j in range(bo,len(s)):
            if s[j]=='(': dep+=1
            elif s[j]==')':
                dep-=1
                if dep==0: bc=j; break
        args=s[bo+1:bc]
        cast1='(%s)data'%t1 if t1.replace(' ','') not in('CScriptDataBase*',) else 'data'
        cast2='(%s)&game'%t2 if not t2.endswith('&') else '(%s)game'%t2
        if t2.replace(' ','')=='CGameScriptInterfaceBase&': cast2='game'
        nargs=re.sub(r'\b%s\b'%re.escape(n2),'@@2',re.sub(r'\b%s\b'%re.escape(n1),'@@1',args)).replace('@@1',cast1).replace('@@2',cast2)
        rest=s[bc+1:]
        bend=rest.find('{'); ctorbody_end=match_brace(rest,bend)
        if re.search(r'\b(%s|%s)\b'%(re.escape(n1),re.escape(n2)),strip_comments(rest[:ctorbody_end+1])): raise Fail('ctor params used beyond the base initializer')
        s=s[:hm[0].start()]+'@@HEAD@@ '+s[hm[0].end():bo+1]+nargs+s[bc:]
        # rename D -> X, wrap
        d=find_class(s,D)
        pre,dbody,post=s[:d['start']],s[d['bo']+1:d['bc']],s[d['end']:]
        rn=lambda x: re.sub(r'\b%s\b'%re.escape(D),X,x)
        if D!=X: pre,dbody,post=rn(pre),rn(dbody),rn(post)
        newbody=('\npublic:' if d['key']=='struct' else '')+dbody
        dtor='??1%s@NScript@@UAE@XZ'%X
        if (dtor in E or dtor in F) and not re.search(r'~%s\s*\('%X,dbody):
            newbody=newbody+'%s    virtual ~%s();%s// %s\n'%('' if newbody.endswith('\n') else '\n',X,' '*4,dtor)
        fwd='class CScriptDataBase;\nclass CGameScriptInterfaceBase;\n'
        if ens==['NScript']:
            newdecl='class %s%s {%s};'%(X,pub_bases(d['bases']),newbody)
            qual=X if enclosing_ns(post,post.find('@@HEAD@@'))==['NScript'] else 'NScript::'+X
            pre=pre  # forward decls must be global
            m0=re.search(r'(?m)^namespace NScript',pre)
        else:
            pre=re.sub(r'(?m)^[ \t]*(?:struct|class)\s+%s\s*;'%re.escape(X),'namespace NScript { class %s; } using NScript::%s;'%(X,X),pre)
            newdecl='namespace NScript {\nclass %s%s {%s};\n}\nusing NScript::%s;'%(X,pub_bases(d['bases']),newbody,X)
            qual='NScript::'+X
        # global forward declarations of the PDB parameter classes before the first use
        first=min([i for i in (s.find('#include'),) if i>=0]+[0])
        s=fwd+pre+newdecl+post
        s=s.replace('@@HEAD@@','%s::%s(%s)\n    :'%(qual,X,SIG))
    except Fail as e:
        res.append((a,'DEFER',str(e))); continue
    lines=s.split('\n'); i=0
    # header: keep the row's comments (they follow the two forward decls)
    hdr=[l for l in rowsrc(ent[a]['Source']).split('\n')]
    j=0
    while j<len(hdr) and hdr[j].startswith('//'): j+=1
    old_hdr=[h for h in hdr[:j] if not h.startswith('// Leaf:')]
    l1=old_hdr[0] if old_hdr else ''
    desc=l1.split(' -- ',1)[1] if ' -- ' in l1 else re.sub(r'^//\s*','',l1)
    evf,evs=EV.get(a,('',''))
    nh=['// %s(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- %s'%(name,desc),'// Leaf: %s'%t]+old_hdr[1:]+[
        '// Naming wave 35: identity %s (%s); PDB %s in %s.'%(short_ev(evs),'/'.join(evf.split('/')[-2:]),t,pdbwhere(t)),
        '//   Was `%s`; the class is renamed into NScript and takes the PDB parameter types, handed to the row\'s'%sym,
        '//   argument-carrying base model unchanged (data = the first argument, stored at +0x44; the interface at +0x40); same bytes.']
    body=s
    # strip the original leading comment block (now after the forward decls)
    bl=body.split('\n')
    k2=2
    while k2<len(bl) and bl[k2].startswith('//'): k2+=1
    body='\n'.join(bl[:2]+bl[k2:])
    final='\n'.join(nh)+'\n'+body
    newmod='NScript::%s'%X; base=fname(newmod)[:80]+'_'+X[:48]+'_'+a
    nsrc=os.path.dirname(ent[a]['Source'])+'/'+base+'.cpp'; ntest=os.path.dirname(ent[a]['TestSource'])+'/'+base+'_test.cpp'
    p=os.path.join(out,'src',nsrc); os.makedirs(os.path.dirname(p),exist_ok=True)
    open(p,'w').write(final)
    res.append((a,'OK',json.dumps(dict(path=p,leaf=t,module=newmod,new_src=nsrc,new_test=ntest,old_src=ent[a]['Source'],old_test=ent[a]['TestSource'],old_sym=sym,fam='lvlctor'))))
with open(out+'/gen.tsv','w') as f:
    for r in res: f.write('\t'.join(r)+'\n')
import collections; print(collections.Counter(r[1] for r in res))
for r in res:
    if r[1]!='OK': print(r)
