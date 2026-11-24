# filt.py: the free filters on work/cls2.tsv SIGEQ / ACCESS lines (no compile): per (entry, binder, made-up identifier)
#   HDR  = the made-up identifier is declared in some rebuild/include header (an existing-line header edit: defer)
#   DEF  = some landed source defines Scope::Ident( (a self row / same-TU body under the made-up name)
#   STALE= the identifier is absent from the binder's comment-stripped code
#   VIRT = the PDB form is virtual (U/M access code) where the stand-in is not
#   PRIV = the PDB form is private/protected
import re,os,sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF299'
R='/home/user/FableDecomp/rebuild/'
ps=open(R+'build_candidates.ps1',encoding='utf-8').read()
def src(a):
    i=ps.find("Address = '%s'"%a)
    if i<0: return None
    m=re.search(r"\bSource = '([^']+)'",ps[i:ps.find("\n    }",i)]); return R+'src/compiled/'+m.group(1) if m else None
inc={}
for root,_,fs in os.walk(R+'include'):
    for f in fs:
        try: inc[os.path.join(root,f)]=open(os.path.join(root,f),encoding='utf-8',errors='replace').read()
        except Exception: pass
allsrc={}
for root,_,fs in os.walk(R+'src/compiled'):
    for f in fs:
        if f.endswith('.cpp'): allsrc[os.path.join(root,f)]=None
def code(t): return '\n'.join(l.split('//')[0] for l in t.splitlines())
cache={}
def defsearch(scope,ident):
    key=(scope,ident)
    if key in cache: return cache[key]
    cls=scope.split('@')[0]
    rx=re.compile(r'\b%s\s*::\s*%s\s*\('%(re.escape(cls),re.escape(ident)))
    hits=[]
    for p in allsrc:
        t=open(p,encoding='utf-8',errors='replace').read()
        if ident not in t: continue
        if rx.search(code(t)): hits.append(p.split('/')[-1][-12:-4])
    cache[key]=hits; return hits
for l in open(A+'/work/cls2.tsv'):
    n,t,k,w,si,pdb=l.rstrip('\n').split('\t')
    if k=='SIGDIFF': continue
    m=re.match(r'\?([^@]+)@(.*?)@@(.)',si); mp=re.match(r'\?([^@]+)@(.*?)@@(.)',pdb)
    if not m: continue
    ident,scope=m.group(1),m.group(2); cls=scope.split('@')[0]
    fl=[]
    hd=[os.path.relpath(p,R+'include') for p,txt in inc.items() if re.search(r'\b%s\b'%re.escape(ident),code(txt)) and re.search(r'\b%s\b'%re.escape(cls),txt)]
    if hd: fl.append('HDR:'+','.join(hd[:2]))
    d=defsearch(scope,ident)
    if d: fl.append('DEF:'+','.join(d[:3]))
    f=src(w)
    if not f: fl.append('NOSRC')
    else:
        c=code(open(f,encoding='utf-8',errors='replace').read())
        if not re.search(r'\b%s\b'%re.escape(ident),c): fl.append('STALE')
    if mp and mp.group(3) in 'UM' and m.group(3) not in 'UM': fl.append('VIRT')
    if mp and mp.group(3) in 'ABCDIJKL': fl.append('PRIV')
    print('\t'.join([n,t,w,ident,cls,' '.join(fl) or 'CLEAN',mp.group(1) if mp else '']))
