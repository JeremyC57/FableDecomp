# requal.py: re-host a landed source's graded function onto its proven NScript member, keeping the source's own layout model
import re
from csrc import *
VPTR_LINE=re.compile(r'^[ \t]*(?:const\s+)?void\s*\*\s*(?:__vftable|_vptr|VTable|Vtable|vtable|vtbl|m_vtbl|vfptr|__vfptr|_vtable|vptr|m_pVtbl|m_vtable)\s*;[^\n]*\n',re.M)
PAD_LINE=re.compile(r'^([ \t]*)(?:unsigned\s+)?char\s+(\w+)\s*\[\s*(0x[0-9a-fA-F]+|\d+)\s*\]\s*;([^\n]*)\n',re.M)
M0_LINE=re.compile(r'^[ \t]*(?:long|int|unsigned long|unsigned int)\s+(?:m_0|m0|m_00|field0|f0|f00|m_vft)\s*;[^\n]*\n',re.M)
POLY_BASES={'CEntityScriptBase','CScriptBase','CLevelScriptBase','CBaseClass','CBaseClassNonCopyable','CActiveEntityScriptBase'}
class Fail(Exception): pass
def first_member_span(body):
    """span (start,end) of the first declaration statement in a class body (skips blanks, comments, access labels)"""
    i=0; n=len(body)
    while i<n:
        if body[i].isspace(): i+=1; continue
        if body.startswith('//',i): j=body.find('\n',i); i=n if j<0 else j+1; continue
        if body.startswith('/*',i): j=body.find('*/',i); i=n if j<0 else j+2; continue
        m=re.match(r'(public|private|protected)\s*:(?!:)',body[i:])
        if m: i+=m.end(); continue
        j=body.find(';',i)
        if j<0: return None
        e=j+1
        # keep a trailing same-line comment + newline with the statement
        k=body.find('\n',e)
        rest=body[e:k] if k>=0 else body[e:]
        if rest.strip()=='' or rest.strip().startswith('//'): e=(k+1) if k>=0 else n
        return i,e
    return None
def is_poly(s,d,seen=0):
    body=strip_comments(s[d['bo']+1:d['bc']])
    if re.search(r'\bvirtual\b',body): return True
    if d['bases'] and seen<4:
        for b in re.findall(r'(?:(?:public|private|protected|virtual)\s+)*([\w:]+(?:<[^>]*>)?)',d['bases']):
            bn=b.split('<')[0].split('::')[-1]
            bd=find_class(s,bn)
            if bd:
                if is_poly(s,bd,seen+1): return True
            elif bn in POLY_BASES: return True
    return False
def make_poly(s,d,cn):
    """make class `cn` polymorphic without moving its members: drop the vptr placeholder (own, or its first local base's)"""
    body=s[d['bo']+1:d['bc']]
    fm=first_member_span(body)
    tgt=None
    if d['bases']:
        b=re.findall(r'(?:(?:public|private|protected)\s+)?([\w:]+)',d['bases'])[0].split('::')[-1]
        bd=find_class(s,b)
        if bd is None: raise Fail('non-polymorphic header base %s'%b)
        bb=s[bd['bo']+1:bd['bc']]
        if strip_comments(bb).strip()=='' or not re.search(r'[\w\]]\s*;',strip_comments(re.sub(r'\([^()]*\)\s*(const)?\s*;','',bb))):
            # empty (data-less) local base: the derived's own first member is the vptr placeholder. A virtual added to the
            # derived alone would put the vptr before the empty base (this->base adjusts by 4), so the base stand-in takes it.
            new=make_poly_text(body,fm)
            if new is None: raise Fail('first member of %s is not a vptr placeholder'%cn)
            s2=s[:d['bo']+1]+new+s[d['bc']:]
            bd=find_class(s2,b)
            ins='\n    virtual ~%s();%s// retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)\n'%(b,' '*max(1,20-len(b)))
            s2=s2[:bd['bo']+1]+ins+s2[bd['bo']+1:]
            return s2,'own, base %s made polymorphic'%b
        else:
            fm2=first_member_span(bb)
            new=make_poly_text(bb,fm2)
            if new is None: raise Fail('base %s first member is not a vptr placeholder'%b)
            # the base stand-in takes the vptr (a virtual added to the derived alone would put it before the base: this->base +4)
            ins='virtual ~%s();%s// retail slot 0 (was the vptr placeholder: the stand-in is polymorphic, as retail)\n'%(b,' '*max(1,20-len(b)))
            new=new[:fm2[0]]+ins+new[fm2[0]:]
            return s[:bd['bo']+1]+new+s[bd['bc']:], 'base %s made polymorphic'%b
    else: tgt=(d,body,fm)
    d_,body,fm=tgt
    new=make_poly_text(body,fm)
    if new is None: raise Fail('first member of %s is not a vptr placeholder'%cn)
    return s[:d['bo']+1]+new+s[d['bc']:], 'own'
VPTR_NAME=re.compile(r'^(?:__vftable|__vfptr|_?vptr|vfptr|v_?tbl|vtable|VTable|Vtable|m_vtbl|m_vtable|m_pVtbl|ScriptVptr|\w*[Vv]ptr|\w*V[Tt]able|pad|_pad|f0|f00|field0|m_0|m0|m_00|m_vft)$')
def make_poly_text(body,fm):
    if fm is None: return None
    st=body[fm[0]:fm[1]]
    stmt=st.split(';')[0].strip()
    m=re.match(r'^(?:const\s+)?void\s*\*\s*(\w+)$',stmt) or re.match(r'^(?:long|int|unsigned long|unsigned int|unsigned)\s+(\w+)$',stmt)
    if m and VPTR_NAME.match(m.group(1)):
        return body[:fm[0]]+body[fm[1]:]
    m=re.match(r'^(?:unsigned\s+)?char\s+(\w+)\s*\[\s*(0x[0-9a-fA-F]+|\d+)\s*\]$',stmt)
    if m:
        nn=int(m.group(2),0)
        if nn<4: return None
        if nn==4: return body[:fm[0]]+body[fm[1]:]
        return body[:fm[0]]+'unsigned char %s[0x%x];   // (-4: now the vptr of the virtual member)\n'%(m.group(1),nn-4)+body[fm[1]:]
    return None
def ns_spans(s):
    """[(name, open_brace, close_brace)] of namespace blocks"""
    out=[]
    for m in re.finditer(r'\bnamespace\s+(\w+)\s*\{',s):
        ls=s.rfind('\n',0,m.start())+1
        if '//' in s[ls:m.start()]: continue
        bo=m.end()-1; out.append((m.group(1),bo,match_brace(s,bo)))
    return out
def enclosing_ns(s,pos):
    return [n for n,bo,bc in ns_spans(s) if bo<pos<bc]
def pub_bases(bases):
    if not bases: return ''
    parts=[p.strip() for p in re.split(r',(?![^<]*>)',bases)]
    return ' : '+', '.join(p if re.match(r'^(public|private|protected)\b',p) else 'public '+p for p in parts)
def requalify(s,cn,cm,classes,TM,acc,free=False,pdbsym=''):
    notes=[]
    leaf=classes[-1]; O=classes[0] if len(classes)==2 else None
    d=find_class(s,cn)
    if d is None and O and cn==leaf:
        m=re.search(r'(?<!\w)(struct|class)\s+%s\s*::\s*%s\b\s*(:[^{;()]*)?\{'%(re.escape(O),re.escape(leaf)),s)
        if m and enclosing_ns(s,m.start())==['NScript']:
            return qualified_fix(s,m,cn,cm,O,leaf,TM,acc,pdbsym)
    if d is None: raise Fail('class %s not found'%cn)
    ens=enclosing_ns(s,d['start'])
    if ens and ens!=['NScript']: raise Fail('class %s inside namespace %s'%(cn,ens))
    in_ns=ens==['NScript']
    nested_ok=False
    if in_ns and O:
        od0=find_class(s,O)
        if od0 and od0['bo']<d['start']<od0['bc'] and cn==leaf: nested_ok=True
        elif od0 and od0['bc']<d['start'] and cn==leaf and enclosing_ns(s,od0['start'])==['NScript']: nested_ok='nest'
        else: raise Fail('entity class %s inside NScript but not nested in %s'%(cn,O))
    poly0=is_poly(s,d)
    # 1 method declaration
    body=s[d['bo']+1:d['bc']]
    if not free:
        pat=re.compile(r'^[ \t]*(?:virtual\s+)?void\s+%s\s*\(\s*(?:void)?\s*\)\s*;[^\n]*\n'%re.escape(cm),re.M)
        if len(pat.findall(body))==1: body=pat.sub('',body)
        else:
            pat2=re.compile(r'(?:virtual\s+)?void\s+%s\s*\(\s*(?:void)?\s*\)\s*;'%re.escape(cm))
            if len(pat2.findall(body))!=1: raise Fail('declaration of %s::%s not found once'%(cn,cm))
            body=pat2.sub('',body)
        if re.search(r'\b%s\s*\('%re.escape(cm),strip_comments(body)) and cm!=TM: raise Fail('%s still referenced in class'%cm)
    s=s[:d['bo']+1]+body+s[d['bc']:]
    d=find_class(s,cn)
    # 2 polymorphism
    if not poly0 and not is_poly(s,d):
        s,how=make_poly(s,d,cn); notes.append('vptr placeholder dropped (%s)'%how)
        d=find_class(s,cn)
    # 3 virtual method at the class end
    ind=d['indent']+'    '
    tail='%s%s:\n%svirtual void %s();%s// PDB %s\n'%('' if s[d['bc']-1]=='\n' else '\n',d['indent']+('    ' if O else '')+acc if False else acc,ind,TM,' '*max(1,30-len(TM)),pdbsym)
    tail=('' if s[d['bc']-1]=='\n' else '\n')+'%s:\n%svirtual void %s();%s// PDB %s\n%s'%(acc,ind,TM,' '*max(1,30-len(TM)),pdbsym,d['indent'])
    s=s[:d['bc']]+tail+s[d['bc']:]
    d=find_class(s,cn)
    if nested_ok=='nest':
        # both NScript-level: declare the entity in its outer, define it as O::I in place
        s=re.sub(r'(?m)^[ \t]*(?:struct|class)\s+%s\s*;[^\n]*\n'%re.escape(leaf),'',s)
        od=find_class(s,O)
        ob=s[od['bo']+1:od['bc']]
        s=s[:od['bc']]+('' if ob.endswith('\n') else '\n')+'public:\n    class %s;%s// the entity class below (naming wave 35)\n'%(leaf,' '*max(1,26-len(leaf)))+s[od['bc']:]
        d=find_class(s,cn)
        head='class %s::%s%s {%s'%(O,leaf,pub_bases(d['bases']),'\npublic:' if d['key']=='struct' else '')
        s=s[:d['start']]+head+s[d['bo']+1:]
        hm=list(re.finditer(r'(?m)(?<![\w:])void\s+(?:NScript\s*::\s*)?%s\s*::\s*%s\s*\(\s*(?:void)?\s*\)'%(re.escape(cn),re.escape(cm)),s))
        if len(hm)!=1: raise Fail('nest definition found %d times'%len(hm))
        pfx='' if enclosing_ns(s,hm[0].start())==['NScript'] else 'NScript::'
        s=s[:hm[0].start()]+'void %s%s::%s::%s()'%(pfx,O,leaf,TM)+s[hm[0].end():]
        return s,notes+['NScript-level %s nested into %s'%(leaf,O)]
    if nested_ok:
        hm=list(re.finditer(r'(?m)void\s+(?:NScript\s*::\s*)?(?:%s\s*::\s*)?%s\s*::\s*%s\s*\(\s*(?:void)?\s*\)'%(re.escape(O),re.escape(cn),re.escape(cm)),s))
        if len(hm)!=1: raise Fail('nested definition found %d times'%len(hm))
        s=s[:hm[0].start()]+'void NScript::%s::%s::%s()'%(O,leaf,TM)+s[hm[0].end():]
        d=find_class(s,cn)
        if d['key']=='struct': pass
        return s,notes+['nested class fixed in place']
    # 4 definition head
    if free:
        fm=re.search(r'(?m)^[ \t]*(?:static\s+)?void\s+__fastcall\s+%s\s*\(\s*(?:struct\s+|class\s+)?%s\s*\*\s*(\w+)\s*\)\s*\n?\s*\{'%(re.escape(cm),re.escape(cn)),s)
        if not fm: raise Fail('free definition %s(%s*) not found'%(cm,cn))
        pv=fm.group(1); bo=s.find('{',fm.start()); bc=match_brace(s,bo)
        fb=re.sub(r'\b%s\b'%re.escape(pv),'this',s[bo:bc])
        s=s[:fm.start()]+'@@HEAD@@\n'+fb+s[bc:]
        # prototypes
        s=re.sub(r'(?m)^[ \t]*(?:extern\s+"C"\s+)?(?:static\s+)?void\s+__fastcall\s+%s\s*\([^)]*\)\s*;[^\n]*\n'%re.escape(cm),'',s)
        if cm!=TM and re.search(r'\b%s\b'%re.escape(cm),strip_comments(s)): raise Fail('free function %s still referenced'%cm)
    else:
        hm=list(re.finditer(r'(?m)(^[ \t]*|(?<=[;}{])[ \t]*)void\s+(?:__thiscall\s+)?(?:NScript\s*::\s*)?%s\s*::\s*%s\s*\(\s*(?:void)?\s*\)'%(re.escape(cn),re.escape(cm)),s))
        if len(hm)!=1: raise Fail('definition %s::%s found %d times'%(cn,cm,len(hm)))
        s=s[:hm[0].start()]+hm[0].group(1)+'@@HEAD@@'+s[hm[0].end():]
    # 5 rename the class to the target leaf
    d=find_class(s,cn)
    head_old=s[d['start']:d['bo']]
    pre,decl,post=s[:d['start']],s[d['start']:d['end']],s[d['end']:]
    dbody=decl[d['bo']-d['start']+1:d['bc']-d['start']]
    if cn!=leaf:
        rn=lambda x: re.sub(r'\b%s\b'%re.escape(cn),leaf,x)
        pre,dbody,post=rn(pre),rn(dbody),rn(post)
        notes.append('class %s renamed %s'%(cn,leaf))
    key_struct=d['key']=='struct'
    newbody=('\npublic:' if key_struct else '')+dbody
    # forward declarations of leaf / outer at global scope
    def fix_fwd(txt,name,qual,using):
        return re.sub(r'(?m)^([ \t]*)(?:struct|class)\s+%s\s*;'%re.escape(name),(r'\1namespace NScript { %s }%s'%(qual,(' using NScript::%s;'%name) if using else '')),txt)
    if O:
        pre=re.sub(r'(?m)^[ \t]*(?:struct|class)\s+%s\s*;[^\n]*\n'%re.escape(leaf),'',pre)
        post=re.sub(r'(?m)^[ \t]*(?:struct|class)\s+%s\s*;[^\n]*\n'%re.escape(leaf),'',post)
        od=find_class(pre,O)
        if od is None and find_class(post,O) is not None: raise Fail('outer %s defined after the entity class'%O)
        if od:
            oens=enclosing_ns(pre,od['start'])
            if oens==['NScript']:
                ob=pre[od['bo']+1:od['bc']]
                pre=pre[:od['bc']]+('' if ob.endswith('\n') else '\n')+'public:\n    class %s;%s// the entity class below (naming wave 35)\n'%(leaf,' '*max(1,26-len(leaf)))+pre[od['bc']:]
            elif oens: raise Fail('outer inside namespace %s'%oens)
            else:
                ob=pre[od['bo']+1:od['bc']]
                newo='namespace NScript {\nclass %s%s {%s%s%spublic:\n    class %s;%s// the entity class below (naming wave 35)\n};\n}\nusing NScript::%s;'%(O,pub_bases(od['bases']),'\npublic:' if od['key']=='struct' else '',ob,'' if ob.endswith('\n') else '\n',leaf,' '*max(1,26-len(leaf)),O)
                pre=pre[:od['start']]+newo+pre[od['end']:]
                notes.append('outer stand-in %s moved into NScript'%O)
            pre=fix_fwd(pre,O,'class %s;'%O,True)
        else:
            if re.search(r'(?m)^[ \t]*(?:struct|class)\s+%s\s*;'%re.escape(O),pre):
                pre=fix_fwd(pre,O,'class %s;'%O,True)
            if re.search(r'\b%s\b'%re.escape(O),strip_comments(post.replace('@@HEAD@@',''))) and not re.search(r'\b%s\b'%re.escape(O),strip_comments(pre)):
                pass
            pre=pre+'namespace NScript {\nclass %s {%s// the PDB owner script; only its nested entity class is modelled here\npublic:\n    class %s;\n};\n}\n'%(O,' '*4,leaf)
            if not re.search(r'using NScript::%s;'%O,pre): pre+='using NScript::%s;\n'%O
        newdecl='namespace NScript {\nclass %s::%s%s {%s};\n}'%(O,leaf,pub_bases(d['bases']),newbody)
        qual='NScript::%s::%s'%(O,leaf)
        refs=re.search(r'\b%s\b'%re.escape(leaf),strip_comments(post.replace('@@HEAD@@','')))
        if refs: newdecl+='\ntypedef %s %s;%s// global spelling used below'%(qual,leaf,' '*4)
    else:
        qual='NScript::%s'%leaf
        if in_ns:
            newdecl='class %s%s {%s};'%(leaf,pub_bases(d['bases']),newbody)
        else:
            pre=fix_fwd(pre,leaf,'class %s;'%leaf,True); post=fix_fwd(post,leaf,'class %s;'%leaf,True)
            newdecl='namespace NScript {\nclass %s%s {%s};\n}\nusing NScript::%s;'%(leaf,pub_bases(d['bases']),newbody,leaf)
    s=pre+newdecl+post
    s=s.replace('@@HEAD@@','void %s::%s()'%(qual,TM))
    return s,notes

def qualified_fix(s,m,cn,cm,O,leaf,TM,acc,pdbsym):
    """`class O::I : bases {` already defined in NScript: fix the method declaration (private/public virtual) in place"""
    pq=r'(?<!\w)(struct|class)\s+%s\s*::\s*%s\b\s*(:[^{;()]*)?\{'%(re.escape(O),re.escape(leaf))
    def dd(s):
        m=re.search(pq,s); bo=m.end()-1; bc=match_brace(s,bo)
        return dict(start=m.start(),indent='',bo=bo,bc=bc,end=s.find(';',bc)+1,key=m.group(1),bases=(m.group(2) or '')[1:].strip())
    d=dd(s); notes=[]
    poly0=is_poly(s,d)
    body=s[d['bo']+1:d['bc']]
    pat=re.compile(r'^[ \t]*(?:virtual\s+)?void\s+%s\s*\(\s*(?:void)?\s*\)\s*;[^\n]*\n'%re.escape(cm),re.M)
    if len(pat.findall(body))!=1: raise Fail('qualified: declaration of %s not found once'%cm)
    body=pat.sub('',body); s=s[:d['bo']+1]+body+s[d['bc']:]
    d=dd(s)
    if not poly0:
        s,how=make_poly(s,d,cn); notes.append('vptr placeholder dropped (%s)'%how); d=dd(s)
    body=s[d['bo']+1:d['bc']]
    body=body+('' if body.endswith('\n') else '\n')+'%s:\n    virtual void %s();%s// PDB %s\n'%(acc,TM,' '*max(1,30-len(TM)),pdbsym)
    s=s[:d['bo']+1]+body+s[d['bc']:]
    hm=list(re.finditer(r'(?<![\w:])void\s+(?:NScript\s*::\s*)?%s\s*::\s*%s\s*::\s*%s\s*\(\s*(?:void)?\s*\)'%(re.escape(O),re.escape(leaf),re.escape(cm)),s))
    if len(hm)!=1: raise Fail('qualified definition found %d times'%len(hm))
    pfx='' if enclosing_ns(s,hm[0].start())==['NScript'] else 'NScript::'
    s=s[:hm[0].start()]+'void %s%s::%s::%s()'%(pfx,O,leaf,TM)+s[hm[0].end():]
    return s,notes+['method declaration made %s virtual in place'%acc]
