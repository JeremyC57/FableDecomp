# wavelib.py -- shared helpers for the phase 6 wave tools (plan agent, 2026-11-05).
# A wave directory W holds: overlay/rebuild/src/compiled/.. (changed sources), overlay/rebuild/tests/.. (changed tests),
# overlay/rebuild/include/.. (changed headers), rows.tsv (address<TAB>reason/notes, the candidate list), gate.tsv, grades.tsv, ...
import os,re,sys,json,csv,hashlib,collections
S='/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad'
P=S+'/plan/p7'; IDX=S+'/plan/idx'; ROOT='/home/user/FableDecomp'
sys.path.insert(0,ROOT+'/rebuild/candidates/manual/naming/2026-11-03_wave33/scripts')
from common import ps1_entries, catalog, pdb
_ent=None;_cat=None;_cache=None
def ent():
    global _ent
    if _ent is None: _ent=ps1_entries()
    return _ent
def cat():
    global _cat
    if _cat is None: _cat=catalog()
    return _cat
def cache():
    global _cache
    if _cache is None: _cache=json.load(open(IDX+'/CACHE.json'))
    return _cache
def src_rel(a): return 'rebuild/src/compiled/'+ent()[a]['Source']
def tst_rel(a): return 'rebuild/tests/'+ent()[a]['TestSource'] if ent()[a].get('TestSource') else None
def ov_path(W,rel): return os.path.join(W,'overlay',rel)
def read(p): return open(p,encoding='utf-8',errors='surrogateescape').read()
def write(p,txt):
    os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'w',encoding='utf-8',errors='surrogateescape').write(txt)
def overlay_src(W,a):
    """the row's source text as the wave leaves it (overlay copy, else the repo's)"""
    p=ov_path(W,src_rel(a)); return read(p) if os.path.exists(p) else read(ROOT+'/'+src_rel(a))
def overlay_tst(W,a):
    t=tst_rel(a)
    if not t: return ''
    p=ov_path(W,t); return read(p) if os.path.exists(p) else read(ROOT+'/'+t)
def wave_rows(W):
    """addresses whose source or test is in the overlay"""
    out=set(); bysrc={v['Source']:a for a,v in ent().items()}; bytst={v.get('TestSource'):a for a,v in ent().items()}
    for d,_,fl in os.walk(os.path.join(W,'overlay','rebuild')):
        for f in fl:
            rel=os.path.relpath(os.path.join(d,f),os.path.join(W,'overlay'))
            if rel.startswith('rebuild/src/compiled/'): a=bysrc.get(rel[len('rebuild/src/compiled/'):])
            elif rel.startswith('rebuild/tests/'): a=bytst.get(rel[len('rebuild/tests/'):])
            else: a=None
            if a: out.add(a)
    return sorted(out)
def tsv(p):
    if not os.path.exists(p): return []
    return list(csv.DictReader(open(p,encoding='utf-8',errors='replace'),delimiter='\t',quoting=csv.QUOTE_NONE))
def sha12(line): return hashlib.sha1(line.encode('utf-8','replace')).hexdigest()[:12]
def index_binds():
    """target -> Counter(symbol -> n callers), and symbol -> set(targets), from the index (funclet rows excluded as gate.py does)"""
    c=cat(); T=collections.defaultdict(collections.Counter); callers=collections.defaultdict(set)
    for a,r in cache().items():
        if a in c and 'Unwind@' in c[a][1]: continue
        for s,t,k in r.get('binds',[]): T[t][s]+=1; callers[(t,s)].add(a)
    return T,callers
# --- C++ text helpers shared by the surveys and the migration rewriter ---
def class_rx(CLS):
    """a `class|struct CLS [: bases] {` definition (not a forward declaration, not a qualified name)"""
    return re.compile(r'(?<![\w:])(class|struct)\s+'+CLS+r'\b\s*(?:/\*.*?\*/\s*)?(?::\s*([^{;]*))?\{')
def blank_comments(t):
    """same length as t, comments and string literals blanked (for brace scanning)"""
    out=list(t); i=0; n=len(t)
    while i<n:
        if t.startswith('//',i):
            j=t.find('\n',i); j=n if j<0 else j
            for k in range(i,j): out[k]=' '
            i=j
        elif t.startswith('/*',i):
            j=t.find('*/',i+2); j=n if j<0 else j+2
            for k in range(i,j): out[k]=' '
            i=j
        elif t[i]=='"' or t[i]=="'":
            q=t[i]; j=i+1
            while j<n and t[j]!=q:
                if t[j]=='\\': j+=1
                j+=1
            for k in range(i+1,min(j,n)): out[k]=' '
            i=j+1
        else: i+=1
    return ''.join(out)
def block(txt,start,b=None):
    """txt[start:end] of the brace block whose `{` is at start (comment-aware when b=blank_comments(txt) is given)"""
    b=b or txt; d=0; i=start
    while i<len(b):
        c=b[i]
        if c=='{': d+=1
        elif c=='}':
            d-=1
            if d==0: return txt[start:i+1]
        i+=1
    return txt[start:]
def namespace_at(t,pos,b=None):
    """names of the namespaces enclosing offset pos"""
    b=b or blank_comments(t); st=[]
    for m in re.finditer(r'\bnamespace\s+(\w+)\s*\{|[{}]',b):
        if m.start()>=pos: break
        if m.group(0)=='{': st.append(None)
        elif m.group(0)=='}':
            if st: st.pop()
        else: st.append(m.group(1))
    return [x for x in st if x]
