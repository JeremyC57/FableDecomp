# respell.py <tag> [caller...] : build caller respell candidates from DECISIONS_V1.tsv (winner per target), compile, verify, report.
#   candidate = landed source with whole-word identifier renames derived by aligning the demangled loser and winner spellings.
#   accept iff: compiles; graded block bytes identical to the baseline; the defined symbol is unchanged or becomes its own decided
#   winner; every changed relocation now names the decided winner of its target (no other relocation changes); >=1 loser fixed.
import sys,os,subprocess,struct,shutil
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from vcommon import *
sys.path.insert(0,ROOT+'/tools/decomp_pipeline'); sys.path.insert(0,ROOT+'/tools/decomp_pipeline/naming')
os.chdir(ROOT)
import verify_and_land as V, reloc_identity as RI
import importlib.util as _iu
_sp=_iu.spec_from_file_location('nmd',ROOT+'/tools/decomp_pipeline/naming/msvc_demangle.py'); M=_iu.module_from_spec(_sp); _sp.loader.exec_module(M)
from concurrent.futures import ThreadPoolExecutor
def _rt(exe,args,e,_t=120):
    cmd=([V.WINE] if V.WINE else [])+[str(exe)]+V.tool_args(args)
    try: return subprocess.run(cmd,capture_output=True,text=True,errors='replace',env=e,timeout=_t)
    except subprocess.TimeoutExpired: return subprocess.CompletedProcess(cmd,124,'timeout','')
V.run_tool=_rt
tag=sys.argv[1]; only=set(sys.argv[2:])
OUT=W+'/rs_'+tag; os.makedirs(OUT,exist_ok=True)
ent=ps1_entries(); cat=catalog(); E=V.env()
os.environ['FABLE_HDR_OVERLAY']=W+'/overlay/rebuild/include'; E=V.env()
win={}; grade={}
for l in list(open(W+'/DECISIONS_V1.tsv'))[1:]:
    q=l.rstrip('\n').split('\t')
    if q[2] in ('proven','placeholder') and q[1]!='-': win[q[0]]=q[1]; grade[q[0]]=q[2]
# baseline pairs per caller
base=collections.defaultdict(list)
for l in open(W+'/logs/scan_base.tsv'):
    q=l.rstrip('\n').split('\t')
    if q[1]=='OK' and q[2]: base[q[0]].append((q[2],q[3]))
work=collections.defaultdict(list)   # caller -> [(target, loser, winner)]
for c,ps in base.items():
    for s,t in set(ps):
        if t in win and s!=win[t]: work[c].append((t,s,win[t]))
DEFS={}
for l in open(W+'/logs/defs_base.tsv'):
    a,st,s=l.rstrip('\n').split('\t')
    if st in ('OK','MERGED') and s: DEFS[a]=s
for t,w in win.items():
    if t in DEFS and DEFS[t]!=w and t in ent: work[t].append((t,DEFS[t],w))
KW=set('class struct union enum const volatile unsigned signed char short int long float double void bool __int64 wchar_t __thiscall __cdecl __stdcall __fastcall public private protected virtual static far near'.split())
def canon(sym):
    try: d=M.demangle(sym)
    except Exception: return None
    if not isinstance(d,dict) or not d.get('ok'): return None
    return ' '.join([d.get('kind',''),d.get('conv',''),d.get('this_cv',''),d.get('storage',''),'R',d.get('ret',''),'Q',d.get('qual',''),'P']+list(d.get('params',[])))
TOK=re.compile(r'[A-Za-z_][A-Za-z0-9_]*|[^\sA-Za-z0-9_]')
def align(lo,wi):
    a,b=canon(lo),canon(wi)
    if a is None or b is None: return None,'undemangled'
    ta,tb=TOK.findall(a),TOK.findall(b)
    if len(ta)!=len(tb): return None,'shape differs'
    mp={}
    for x,y in zip(ta,tb):
        if x==y: continue
        if not re.match(r'[A-Za-z_]',x) or not re.match(r'[A-Za-z_]',y) or x in KW or y in KW: return None,'shape differs (%s/%s)'%(x,y)
        if mp.get(x,y)!=y: return None,'inconsistent map'
        mp[x]=y
    return mp,'ok'
def blockinfo(obj,leaf,a):
    try: bts,sec,sym=V.obj_text(obj,leaf)
    except Exception as ex: return None
    ln=int(cat[a][2]); b=bytes.fromhex(cat[a][3].strip()); base_=int(a,16)
    rel=RI.function_relocs('objdump',obj,sym,ln) or []
    pr=[]
    for off,kind,s in rel:
        if off<0 or off+4>len(b) or s.startswith(RI.SKIP_PREFIXES): continue
        t=(base_+off+4+struct.unpack_from('<i',b,off)[0])&0xffffffff if kind in ('REL32','DISP32') else struct.unpack_from('<I',b,off)[0]
        pr.append((off,s,'%08x'%t))
    return bts,sym,pr
def compile_(a,txt,d):
    os.makedirs(d,exist_ok=True); f=d+'/x.cpp'; open(f,'w').write(txt)
    fl=(ent[a].get('CompilerFlags') or '/O2 /Oy /W3').split()
    o=d+'/x.obj'
    if os.path.exists(o): os.remove(o)
    r=V.cl(['/nologo','/c']+fl+['/Fo'+o,f],E)
    return o if r.returncode==0 and os.path.exists(o) else None
def one(c):
    J=work[c]; own=[j for j in J if j[0]==c]; oth=[j for j in J if j[0]!=c]
    vs=[J]+([oth] if own and oth else [])+([own] if own and oth else [])+([[j] for j in oth] if len(oth)>1 else [])
    best=None
    for k,jobs in enumerate(vs):
        r=one_(c,jobs,k)
        if r[1]=='OK':
            n=r[3].count(',')
            if best is None or n>best[0]: best=(n,r)
            if k==0: break
        elif k==0: first=r
    return best[1] if best else first
SEG=re.compile(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'',re.S)
def rename(txt,mp,meth,own_job):
    def sub(seg,keys):
        for k in keys: seg=re.sub(r'(?<![A-Za-z0-9_])'+re.escape(k)+r'(?![A-Za-z0-9_])',mp[k],seg)
        return seg
    allk=list(mp); clsk=[k for k in mp if k not in meth]
    out=[]; pos=0; first=True
    for m in SEG.finditer(txt):
        out.append(sub(txt[pos:m.start()],allk))
        g=m.group(0)
        if g.startswith('//'):
            ln_start=txt.rfind('\n',0,m.start())+1
            leafline=g.startswith('// Leaf:')
            line1=(ln_start==0) and own_job
            out.append(sub(g,allk if (leafline or line1) else clsk))
        elif g.startswith('/*'): out.append(sub(g,clsk))
        else: out.append(g)
        pos=m.end()
    out.append(sub(txt[pos:],allk))
    return ''.join(out)
def one_(c,jobs,VK):
    J=jobs; src=ROOT+'/rebuild/src/compiled/'+ent[c]['Source']; txt=open(src,errors='replace').read()
    mp={}; notes=[]
    for t,lo,wi in jobs:
        m,why=align(lo,wi)
        if m is None: notes.append('%s:%s'%(t,why)); continue
        if any(mp.get(k,v)!=v for k,v in m.items()) or any(v in mp and mp[v]!=v for v in m.values()): notes.append('%s:map conflict'%t); continue
        mp.update(m)
    if not mp: return (c,'NOMAP',';'.join(notes),'')
    meth=set()
    for t,lo,wi in J:
        try:
            d=M.demangle(lo)
            if isinstance(d,dict) and d.get('ok'): meth.add(d['comps'][-1].lstrip('~'))
        except Exception: pass
    own_job=any(t==c for t,lo,wi in J)
    new=rename(txt,mp,meth,own_job)
    fixed_note=[]
    lf=leaf_of(txt); nlf=leaf_of(new)
    bo=W+'/cc/'+c+'/x.obj'
    b0=blockinfo(bo,lf,c)
    o=compile_(c,new,OUT+'/'+c+'_%d'%VK)
    if not o: return (c,'COMPILE_FAIL',json.dumps(mp),';'.join(notes))
    b1=blockinfo(o,nlf,c)
    if not b0 or not b1: return (c,'NOBLOCK',json.dumps(mp),'')
    if b0[0]!=b1[0]: return (c,'BYTES_DIFFER',json.dumps(mp),'')
    if b1[1]!=b0[1] and win.get(c)!=b1[1]: return (c,'OWN_SYMBOL_CHANGED',json.dumps(mp),'%s -> %s'%(b0[1],b1[1]))
    if len(b0[2])!=len(b1[2]): return (c,'RELOC_COUNT',json.dumps(mp),'')
    fixed=set(); bad=[]
    for (o0,s0,t0),(o1,s1,t1) in zip(b0[2],b1[2]):
        if s0==s1: continue
        if win.get(t1)==s1: fixed.add(t1)
        else: bad.append('%s:%s->%s'%(t1,s0,s1))
    if bad: return (c,'STRAY_CHANGE',json.dumps(mp),' ; '.join(bad[:4]))
    if b1[1]!=b0[1] and win.get(c)==b1[1]: fixed.add(c+'(own)')
    if not fixed: return (c,'NOFIX',json.dumps(mp),'')
    rest=[t for t,lo,wi in jobs if t not in fixed and t+'(own)' not in fixed]
    note='// Vendor naming wave V1 (one address, one name): '+'; '.join('0x%s %s (was %s)'%(t.upper(),wi,lo) for t,lo,wi in J if (t in fixed or t+'(own)' in fixed))
    L=new.split('\n'); li=[i for i,l in enumerate(L) if l.startswith('// Leaf:')]
    k_=li[0]+1 if li else (1 if L and L[0].startswith('//') else 0)
    L.insert(k_,note); new='\n'.join(L)
    open(OUT+'/'+c+'.cpp.%d'%VK,'w').write(new)
    return (c,'OK',json.dumps(mp),'variant %d; fixed %s%s; own %s'%(VK,','.join(sorted(fixed)),('; still '+','.join(sorted(set(rest)))) if rest else '',b1[1]))
cs=sorted(c for c in work if c in ent and (not only or c in only))
with ThreadPoolExecutor(int(os.environ.get('NJ','6'))) as ex: R=list(ex.map(one,cs))
for r in R:
    if r[1]=='OK':
        k=int(re.search(r'variant (\d+)',r[3]).group(1)); shutil.copy(OUT+'/'+r[0]+'.cpp.%d'%k,OUT+'/'+r[0]+'.cpp')
with open(OUT+'/result.tsv','w') as f:
    for r in R: f.write('\t'.join(r)+'\n')
print(len(cs),'callers',collections.Counter(r[1] for r in R))
