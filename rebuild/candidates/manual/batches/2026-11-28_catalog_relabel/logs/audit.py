import re,os,sys,json,subprocess,collections
S=sys.argv[1]
ps=open('rebuild/build_candidates.ps1',encoding='utf-8-sig').read()
ents=[dict(re.findall(r"(\w+) = '([^']*)'",b)) for b in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}",ps,re.S)]
cat={l.split('\t')[0].lower():l.split('\t') for l in open('rebuild/oracles/auto-re-candidates.tsv',encoding='utf-8').read().splitlines()[1:] if l}
rec=json.load(open(S+'/ns_480/rec.json'))
leafs=[]
for e in ents:
    if 'Address' not in e: continue
    a=e['Address'].lower(); s='rebuild/src/compiled/'+e.get('Source','')
    if not os.path.exists(s) or a not in cat: continue
    m=re.search(r'^//\s*Leaf:\s*(\S+)',open(s,encoding='utf-8',errors='replace').read(),re.M)
    leafs.append((a,m.group(1) if m else '',cat[a][1],s))
mang=[x[1] for x in leafs if x[1].startswith('?')]
out=subprocess.run(['llvm-undname'],input='\n'.join(mang)+'\n',capture_output=True,text=True).stdout
dm={}
for g in out.split('\n\n'):
    ls=[l for l in g.split('\n') if l!='']
    if len(ls)==2: dm[ls[0]]=ls[1]
def strip_alloc(s):
    # drop default allocator / less / char_traits template args
    prev=None
    while prev!=s:
        prev=s
        s=re.sub(r',\s*std::allocator<(?:[^<>]|<(?:[^<>]|<[^<>]*>)*>)*>\s*(?=>)','',s)
        s=re.sub(r',\s*std::less<(?:[^<>]|<(?:[^<>]|<[^<>]*>)*>)*>\s*(?=,|>)','',s)
    return s
def norm(s):
    s=re.sub(r'\b(class|struct|union|enum)\s+','',s)
    s=s.replace("`scalar deleting dtor'","`scalar deleting destructor'").replace("`vector deleting dtor'","`vector deleting destructor'")
    s=s.replace('scalar_deleting_destructor','scalar deleting destructor').replace('`scalar deleting destructor\'','`scalar deleting destructor\'')
    s=re.sub(r'^_global::','',s)
    s=strip_alloc(s)
    s=re.sub(r'\s+','',s)
    s=s.replace("`","").replace("'","")
    # ctor/dtor template suffix: X<..>::X<..> -> X<..>::X
    s=re.sub(r'::(~?)([A-Za-z_]\w*)<.*>$',lambda m:'::'+m.group(1)+m.group(2),s) if re.search(r'::~?\w+<.*>$',s) else s
    return s
def qual(u):
    q=re.search(r'(?:__thiscall|__cdecl|__stdcall|__fastcall) (.*?)\((?!\))',u) or re.search(r'(?:__thiscall|__cdecl|__stdcall|__fastcall) (.*)$',u)
    if not q: return None
    q=q.group(1)
    q=re.sub(r'\(.*$','',q) if not q.endswith("'") else q
    return q
res=collections.Counter(); out=[]
for a,lf,nm,s in leafs:
    r=rec.get(a,{}); st=(r.get('status',''),r.get('tier',''))
    if not lf: res['no leaf line']+=1; continue
    if not lf.startswith('?'):
        q=re.sub(r'@\d+$','',lf); nm2=re.sub(r'^[A-Za-z0-9_]+\.(DLL|dll)::|^lib\w+::','',nm)
        same = q in (nm2, '_'+nm2, nm2.split('::')[-1], '_'+nm2.split('::')[-1]) or q.lstrip('_')==nm2.split('::')[-1].lstrip('_') or norm(q)==norm(nm2)
    else:
        u=dm.get(lf)
        if not u: res['undemangled']+=1; continue
        q=qual(u)
        if q is None: res['noqual']+=1; continue
        same = norm(q)==norm(re.sub(r'\(.*$','',nm))
    if same: res['same']+=1; continue
    out.append((a,nm,q,lf,st[0],st[1],r.get('pdb_spelling',''),r.get('fix',''),s))
    res['diff']+=1
print(res)
c=collections.Counter((x[4],x[5]) for x in out)
for k,v in c.most_common(): print(v,k)
open(S+'/cn/diff.tsv','w').write(''.join('\t'.join(x)+'\n' for x in out))
