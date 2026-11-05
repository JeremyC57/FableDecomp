# w7_respell.py W JOBS.tsv : plan phase 7 wave 1, the mechanical callee / definer respells. JOBS columns: binder, target, old, new.
# Each job is one of (or both):
#   const  old and new differ only in `@@QAE` -> `@@QBE` (the PDB member is const): ` const` is added to the member's declaration
#          in the row's local class block (and to an out-of-class definition `Class::Name(...)` when the row defines it);
#   ns     new adds the namespace NGraphicAppearance to CTCGraphicAppearance (Ego_r.pdb / FableWin.pdb): the row's local
#          CTCGraphicAppearance model (block or forward declaration) moves into `namespace NGraphicAppearance { }` with a
#          using-declaration after it.
# The row's text is taken from W's overlay when an earlier job of this run already changed it. Writes W/overlay/<src> and
# W/rows.tsv (address, rule). gate.py decides the bytes and reports the respell (old -> new) per relocation.
import sys,os,re,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__))); import wavelib as L
W=sys.argv[1]; jobs=[l.rstrip('\n').split('\t') for l in open(sys.argv[2]) if l.strip() and not l.startswith('binder')]
os.makedirs(W,exist_ok=True)
def text(a):
    p=L.ov_path(W,L.src_rel(a)); return L.read(p) if os.path.exists(p) else L.read(L.ROOT+'/'+L.src_rel(a))
def parse(sym):
    m=re.match(r'^\?(\w+)@((?:\w+@)+)@([A-Z])([A-Z])E',sym)
    return (m.group(1),m.group(2).rstrip('@').split('@')) if m else (None,None)
def add_const(t,cls,name):
    b=L.blank_comments(t); n=0
    for mc in list(L.class_rx(cls).finditer(b))[::-1]:
        s=b.find('{',mc.start()); blk=L.block(t,s,b); bb=b[s:s+len(blk)]
        new=blk; nb=bb
        for md in list(re.finditer(r'(?<![\w:~])'+re.escape(name)+r'\s*\(',nb))[::-1]:
            d=1; j=md.end()
            while j<len(nb) and d>0:
                if nb[j]=='(': d+=1
                elif nb[j]==')': d-=1
                j+=1
            k=j
            while k<len(nb) and nb[k] in ' \t': k+=1
            if nb.startswith('const',k): continue
            if k<len(nb) and (nb[k] in ';{' or nb.startswith('=',k)):
                new=new[:j]+' const'+new[j:]; nb=nb[:j]+' const'+nb[j:]; n+=1
        t=t[:s]+new+t[s+len(blk):]; b=L.blank_comments(t)
    # out-of-class definitions Class::Name(...)
    b=L.blank_comments(t)
    for md in list(re.finditer(r'(?<![\w:])'+re.escape(cls)+r'\s*::\s*'+re.escape(name)+r'\s*\(',b))[::-1]:
        d=1; j=md.end()
        while j<len(b) and d>0:
            if b[j]=='(': d+=1
            elif b[j]==')': d-=1
            j+=1
        k=j
        while k<len(b) and b[k] in ' \t\n': k+=1
        if b.startswith('const',k): continue
        if k<len(b) and b[k]=='{': t=t[:j]+' const'+t[j:]; b=L.blank_comments(t); n+=1
    return t,n
def add_ns(t,cls='CTCGraphicAppearance',ns='NGraphicAppearance'):
    b=L.blank_comments(t)
    pass   # an existing namespace block (another class) does not stop the wrap: the namespace is reopened
    n=0
    # global forward declarations first (each keeps its place: earlier uses still see the name)
    for mf in list(re.finditer(r'(?<![\w:])((class|struct)\s+'+cls+r'\s*;)',b))[::-1]:
        if L.namespace_at(t,mf.start(),b) or re.search(r'\bfriend\s*$',b[max(0,mf.start()-12):mf.start()]): continue
        t=t[:mf.start(1)]+'namespace '+ns+' { '+t[mf.start(1):mf.end(1)]+' }\nusing '+ns+'::'+cls+';'+t[mf.end(1):]; n+=1
    b=L.blank_comments(t); mg=L.class_rx(cls).search(b)
    if mg and not L.namespace_at(t,mg.start(),b):
        sg=b.find('{',mg.start()); gb=L.block(t,sg,b); ge=t.find(';',sg+len(gb))+1
        e=t.find('\n',ge); e=len(t) if e<0 else e
        if t[ge:e].strip() and not t[ge:e].strip().startswith('//'): e=ge
        t=t[:mg.start()]+'namespace '+ns+' {\n'+t[mg.start():e]+'\n}\nusing '+ns+'::'+cls+';'+t[e:]; n+=1
    return t,n
res={}
for a,tg,old,new in jobs:
    t=text(a); rule=[]
    on,oc=parse(old); nn,nc=parse(new)
    if '@@QAE' in old and old.replace('@@QAE','@@QBE',1).replace('@NGraphicAppearance','')==new.replace('@NGraphicAppearance',''):
        if on and oc:
            t2,n=add_const(t,oc[0],on); 
            if n: t=t2; rule.append('const %s::%s x%d'%(oc[0],on,n))
            else: rule.append('const NOT_FOUND %s::%s'%(oc[0],on))
    if 'NGraphicAppearance' in new and 'NGraphicAppearance' not in old:
        t2,n=add_ns(t)
        if n: t=t2; rule.append('ns NGraphicAppearance')
        else: rule.append('ns NOT_DONE')
    if t!=text(a): L.write(L.ov_path(W,L.src_rel(a)),t)
    res.setdefault(a,[]).append(tg+': '+'; '.join(rule))
with open(W+'/rows.tsv','w') as o:
    o.write('address\trule\n')
    for a in sorted(res): o.write(a+'\t'+('RESPELL: ' if all('NOT' not in x for x in res[a]) else 'UNCHANGED: ')+' | '.join(res[a])+'\n')
print(collections.Counter(('ok' if all('NOT' not in x for x in v) else 'notdone') for v in res.values()))
