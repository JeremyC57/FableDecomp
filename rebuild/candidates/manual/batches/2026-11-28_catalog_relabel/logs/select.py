# pick catalog-name relabels that are provable: the source leaf == tracker self_leaf, a single spelling or the one PDB spelling
import re,os,sys,json,subprocess,collections,glob
S=sys.argv[1]
ps=open('rebuild/build_candidates.ps1',encoding='utf-8-sig').read()
ents={}
for b in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}",ps,re.S):
    e=dict(re.findall(r"(\w+) = '([^']*)'",b))
    if 'Address' in e: ents[e['Address'].lower()]=e
cat={l.split('\t')[0].lower():l.split('\t')[1] for l in open('rebuild/oracles/auto-re-candidates.tsv',encoding='utf-8').read().splitlines()[1:] if l}
rec=json.load(open(S+'/ns_480/rec.json'))
shared={l.split('\t')[0] for l in open(S+'/ns_480/SHARED_PDB.tsv').read().splitlines()[1:]}
descr={l.split('\t')[0].lower() for l in open('rebuild/oracles/descriptive_names.tsv',encoding='utf-8').read().splitlines() if l and not l.startswith('#')}
# holds
held={}
for r in [l.split('\t') for l in open('rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv').read().splitlines()[1:]]:
    if len(r)>1: held.setdefault(r[1].lower().replace('0x',''),'T3')
    if len(r)>2:
        for x in re.findall(r'0x([0-9a-fA-F]{8})',r[2]): held.setdefault(x.lower(),'T3')
    for x in re.findall(r'0x([0-9a-fA-F]{8})','\t'.join(r[3:4])): held.setdefault(x.lower(),'T3 binder')
for o in glob.glob(S+'/w22/nflocks/*/owner'):
    if open(o).read().strip()!='released': held.setdefault(o.split('/')[-2].lower(),'lock')
for l in open('rebuild/candidates/manual/COORDINATION_CLAIMS.tsv').read().splitlines():
    f=l.split('\t')
    if len(f)>4 and f[4] not in ('done','released'):
        for x in re.findall(r'(?:0x)?([0-9a-fA-F]{8})\b',l): held.setdefault(x.lower(),'open claim')
for p in glob.glob('rebuild/candidates/manual/pending/*/RESERVED.tsv'):
    for l in open(p).read().splitlines()[1:]:
        f=l.split('\t')
        if f and re.fullmatch(r'[0-9a-fA-F]{8}',f[0]) and 'landed' not in l.split('\t')[-1][:7]: held.setdefault(f[0].lower(),'reserved')
def leaf(a):
    e=ents.get(a); s='rebuild/src/compiled/'+e.get('Source','') if e else ''
    if not e or not os.path.exists(s): return ''
    m=re.search(r'^//\s*Leaf:\s*(\S+)',open(s,encoding='utf-8',errors='replace').read(),re.M)
    return m.group(1) if m else ''
cand=[]
for a,r in rec.items():
    if r.get('landed')!='y' or a not in cat or a not in ents: continue
    lf=leaf(a)
    if not lf.startswith('?') or lf!=r.get('self_leaf'): continue
    st,tier=r['status'],r.get('tier','')
    if st=='agree_pdb' and r.get('n_spellings')==1: kind='pdb'
    elif st=='split_fixable' and r.get('pdb_spelling')==lf: kind='pdb-split'
    elif st=='agree_std' and r.get('n_spellings')==1: kind='std'
    else: continue
    cand.append((a,lf,kind))
# demangle one per symbol (pair by echoed input)
syms=sorted({c[1] for c in cand})
dm={}
for g in subprocess.run(['llvm-undname'],input='\n'.join(syms)+'\n',capture_output=True,text=True).stdout.split('\n\n'):
    ls=[l for l in g.split('\n') if l]
    if len(ls)==2 and ls[0] in syms: dm[ls[0]]=ls[1]
def split_chain(q):
    parts=[];d=0;cur=''
    i=0
    while i<len(q):
        ch=q[i]
        if ch=='<' and not cur.endswith('operator') and not re.search(r'operator<?$',cur): d+=1
        elif ch=='>' and d>0: d-=1
        if d==0 and q[i:i+2]=='::': parts.append(cur);cur='';i+=2;continue
        cur+=ch;i+=1
    parts.append(cur); return parts
def strip_t(seg):
    if seg.startswith('operator') or seg.startswith('`'): return seg
    out='';d=0
    for ch in seg:
        if ch=='<': d+=1;continue
        if ch=='>': d-=1;continue
        if d==0: out+=ch
    return out
def clean(s):
    s=re.sub(r'\b(class|struct|enum|union) ','',s); s=s.replace(' *','*').replace(', ',',').replace('>>','> >').replace('>>','> >')
    return s
PLACE=re.compile(r"^_global$|^_unattributed$|^_gapscan$|^CVtable_|_[0-9a-f]{8}$|^sub_|_Methods$|^Unwind@|^vfunc_|Unproven|^CUnknown",re.I)
def qual(u):
    body=re.sub(r'^(public|private|protected): ','',u); body=re.sub(r'^(static |virtual )+','',body)
    m=re.search(r'__(?:thiscall|cdecl|stdcall|fastcall) (.*)$',body)
    if not m: return None
    q=m.group(1)
    # cut the argument list: the last '(' at depth 0 that starts the parameter list
    d=0
    for i,ch in enumerate(q):
        if ch=='<': d+=1
        elif ch=='>': d-=1
        elif ch=='(' and d<=0 and not q[:i].endswith('operator') and not q[:i].endswith('operator()'):
            return q[:i]
    return q
out=[];why=collections.Counter()
for a,lf,kind in cand:
    if a in held: why['held '+held[a]]+=1; continue
    if a in descr: why['descriptive registry']+=1; continue
    if lf in shared: why['shared PDB name']+=1; continue
    u=dm.get(lf)
    if not u: why['no demangle']+=1; continue
    q=qual(u)
    if not q or 'operator' in q: why['operator / unparsed']+=1; continue
    newc=[strip_t(clean(p)) for p in split_chain(q)]
    oldc=[strip_t(p) for p in split_chain(re.sub(r'^_global::','',cat[a]))]
    norm=lambda c:[x.replace("`scalar deleting dtor'","`scalar deleting destructor'").replace("`vector deleting dtor'","`vector deleting destructor'").replace('scalar_deleting_destructor','scalar deleting destructor').replace('`','').replace("'",'').replace(' ','') for x in c]
    if norm(newc)==norm(oldc): why['cosmetic only']+=1; continue
    if kind=='std':
        if not q.startswith('std::') or not any(PLACE.search(x) for x in split_chain(cat[a])): why['std: catalog not a placeholder or leaf not std::']+=1; continue
    # never less specific: every old non-placeholder scope must survive
    lost=[x for x in oldc[:-1] if not PLACE.search(x) and x not in newc]
    if len(newc)<len([x for x in oldc if not PLACE.search(x)]) or lost: why['would lose scope']+=1; continue
    last=newc[-1]
    if not re.match(r"^~?[A-Za-z_]\w*$|^`(scalar|vector) deleting destructor'$",last.replace("`scalar deleting dtor'","`scalar deleting destructor'").replace("`vector deleting dtor'","`vector deleting destructor'")): why['odd last name']+=1; continue
    cls='::'.join(clean(p) for p in split_chain(q)[:-1])
    name=clean(split_chain(q)[-1]).replace("`scalar deleting dtor'","`scalar deleting destructor'").replace("`vector deleting dtor'","`vector deleting destructor'")
    name=strip_t(name) if not name.startswith('`') else name
    out.append((a,kind,cat[a],(cls+'::'+name) if cls else name,cls or '_global',name,lf))
    why['relabel '+kind]+=1
for k,v in why.most_common(): print(v,k)
open(S+'/cn/relabel.tsv','w').write('address\tkind\told\tnew\tclass\tname\tleaf\n'+''.join('\t'.join(x)+'\n' for x in out))
