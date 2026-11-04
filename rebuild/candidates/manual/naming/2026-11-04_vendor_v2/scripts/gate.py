# gate.py -- object-identity gate against the symbol index (plan phase 1, 2026-11-04)
# usage (repo root, lane env sourced):
#   python3 gate.py [--index DIR] [--overlay INCDIR] [--out FILE] [--work DIR] ADDR SRC [ADDR SRC ...]
#   python3 gate.py ... @LIST        (LIST: lines "addr<TAB>source path"; "-" or a missing path = the landed source)
# A row PASSES only if
#   (1) the source compiles with the row's build_candidates.ps1 CompilerFlags (FABLE_HDR_OVERLAY = --overlay, if given),
#   (2) the graded function (its // Leaf:, else the largest function, exactly as symindex.py picks it) has the index's length
#       and the same sha1 with every relocation field masked, and
#   (3) every relocation resolves to the same retail target, in the same order and kind, as the index recorded.
# It reports, per row, the defined symbol and every relocation whose SYMBOL changed (old -> new @ target): the respell itself.
# Baseline: the index entry; a row with none (landed after the index run), or every row under GATE_BASELINE=landed, is measured
#   from its landed source first (two compiles instead of one).
# Extra (never changes the verdict): NAME_CLASH when a new symbol is bound in the index to a different retail target, or is the
# symbol another address defines (one retail address, one name).  gate.py is the fast first filter; vland --recheck-landed
# remains the grade of record.
import sys,os,struct,subprocess,re,hashlib,json,time,collections
sys.path.insert(0,'/home/user/FableDecomp/rebuild/candidates/manual/naming/2026-11-03_wave33/scripts')
from common import ps1_entries, catalog, ROOT
sys.path.insert(0,ROOT+'/tools/decomp_pipeline'); os.chdir(ROOT)
S='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'
args=sys.argv[1:]; IDX=S+'/plan/idx'; OVL=None; OUTF=None; WORK=S+'/plan/p1/gate_work'
while args and args[0].startswith('--'):
    k=args.pop(0); v=args.pop(0)
    if k=='--index': IDX=v
    elif k=='--overlay': OVL=v
    elif k=='--out': OUTF=v
    elif k=='--work': WORK=v
if OVL: os.environ['FABLE_HDR_OVERLAY']=os.path.abspath(OVL)
import verify_and_land as V, reloc_identity as RI
def _rt(exe,a,e,_t=180):
    cmd=([V.WINE] if V.WINE else [])+[str(exe)]+V.tool_args(a)
    try: return subprocess.run(cmd,capture_output=True,text=True,errors='replace',env=e,timeout=_t)
    except subprocess.TimeoutExpired: return subprocess.CompletedProcess(cmd,124,'timeout','')
V.run_tool=_rt
jobs=[]
i=0
while i<len(args):
    a=args[i]
    if a.startswith('@'):
        for l in open(a[1:]):
            q=l.rstrip('\n').split('\t')
            if q and re.match(r'^[0-9a-fA-F]{8}$',q[0]): jobs.append((q[0].lower(),q[1] if len(q)>1 and q[1] not in ('','-') else None))
        i+=1
    else:
        jobs.append((a.lower(),args[i+1] if i+1<len(args) and args[i+1]!='-' else None)); i+=2
ent=ps1_entries(); cat=catalog(); E=V.env()
cache=json.load(open(IDX+'/CACHE.json')) if os.path.exists(IDX+'/CACHE.json') else {}
# global index views for the NAME_CLASH note
sym_t=collections.defaultdict(set); def_at={}
for a,r in cache.items():
    if a in cat and 'Unwind@' in cat[a][1]: continue   # funclet rows: their index binds are not meaningful (post.py)
    if r.get('sym'): def_at.setdefault(r['sym'],a)
    for s,t,k in r.get('binds',[]): sym_t[s].add(t)
def leaf_of(t):
    m=re.search(r'^//\s*Leaf:\s*(\S+)',t,re.M); return m.group(1) if m else None
def measure(a,txt,d):
    os.makedirs(d,exist_ok=True); f=d+'/x.cpp'; open(f,'w').write(txt); o=d+'/x.obj'; leaf=leaf_of(txt)
    if os.path.exists(o): os.remove(o)
    fl=(ent[a].get('CompilerFlags') or '/O2 /Oy /W3').split()
    r=V.cl(['/nologo','/c']+fl+['/Fo'+o,f],E)
    if r.returncode or not os.path.exists(o):
        msg=[x for x in (r.stdout or '').splitlines() if ' error ' in x or 'fatal' in x][:1]
        return 'COMPILE_FAIL '+(msg[0][-160:] if msg else '')
    q=cat[a]; ln=int(q[2]); b=bytes.fromhex(q[3].strip()); base=int(a,16)
    try: fb,sec,sym=V.obj_text(o,leaf)
    except Exception: return 'NO_FUNC'
    rel=RI.function_relocs('objdump',o,leaf,ln) or []
    mb=bytearray(fb); binds=[]
    for off,kind,s in rel:
        if 0<=off and off+4<=len(mb): mb[off:off+4]=b'\0\0\0\0'
        if off<0 or off+4>len(b) or s.startswith(RI.SKIP_PREFIXES): continue
        t=(base+off+4+struct.unpack_from('<i',b,off)[0])&0xffffffff if kind in ('REL32','DISP32') else struct.unpack_from('<I',b,off)[0]
        binds.append([s,'%08x'%t,kind])
    return {'status':'OK' if (not leaf or leaf in sym) else 'LEAFGUESS','sym':sym,'len':len(fb),'sha':hashlib.sha1(bytes(mb)).hexdigest(),'binds':binds}
def one(job):
    a,src=job; t0=time.time()
    if a not in ent or a not in cat: return (a,'FAIL','not a landed catalog row','','',0)
    e=ent[a]; r0=cache.get(a)
    if os.environ.get('GATE_BASELINE')=='landed' or not r0:
        # no index entry (a row landed after the index run) or asked for: measure the landed source now
        r0=measure(a,open(ROOT+'/rebuild/src/compiled/'+e['Source'],errors='replace').read(),WORK+'/'+a+'_base')
        if isinstance(r0,str): return (a,'FAIL','baseline (landed source): '+r0,'','',time.time()-t0)
    if r0['status'] not in ('OK','LEAFGUESS'): return (a,'FAIL','no usable index entry (%s)'%r0.get('status'),'','',0)
    p=src or ROOT+'/rebuild/src/compiled/'+e['Source']
    m1=measure(a,open(p,errors='replace').read(),WORK+'/'+a)
    if isinstance(m1,str): return (a,'FAIL',m1,'','',time.time()-t0)
    fb_len,sha,sym,binds=m1['len'],m1['sha'],m1['sym'],m1['binds']; why=[]
    if fb_len!=r0['len']: why.append('LENGTH %d != index %d'%(fb_len,r0['len']))
    elif sha!=r0['sha']: why.append('BYTES (masked sha1 differs)')
    ot=[(t,k) for s,t,k in r0['binds']]; nt=[(t,k) for s,t,k in binds]
    if ot!=nt:
        dd=[x for x in collections.Counter(nt)-collections.Counter(ot)]+['-'+x[0] for x in collections.Counter(ot)-collections.Counter(nt)]
        why.append('TARGETS differ (%d vs index %d) %s'%(len(nt),len(ot),' '.join(str(x[0]) if isinstance(x,tuple) else x for x in dd[:6])))
    ch=[]
    if sym!=r0['sym']: ch.append('DEF %s -> %s'%(r0['sym'],sym))
    seen=set()
    if ot==nt:
        for (s0,t0_,k0),(s1,t1,k1) in zip(r0['binds'],binds):
            if s0!=s1 and (s0,s1,t1) not in seen: seen.add((s0,s1,t1)); ch.append('%s: %s -> %s'%(t1,s0,s1))
    clash=[]
    for s,t,k in binds:
        o_=sym_t.get(s,set())-{t}
        if o_: clash.append('NAME_CLASH %s bound here to %s, in the index to %s'%(s,t,','.join(sorted(o_))))
        if s in def_at and def_at[s]!=t and not s.startswith('??_7'): clash.append('NAME_CLASH %s -> %s is defined at %s'%(s,t,def_at[s]))
    if sym in def_at and def_at[sym]!=a: clash.append('NAME_CLASH own %s is defined at %s'%(sym,def_at[sym]))
    return (a,'FAIL' if why else 'PASS','; '.join(why),' | '.join(ch),' | '.join(sorted(set(clash))),time.time()-t0)
from concurrent.futures import ThreadPoolExecutor
t0=time.time()
with ThreadPoolExecutor(int(os.environ.get('NJ','4'))) as ex: R=list(ex.map(one,jobs))
out=open(OUTF,'w') if OUTF else sys.stdout
out.write('address\tverdict\treason\tchanges\tname_notes\tsecs\n')
for x in R: out.write('\t'.join(x[:5])+'\t%.2f\n'%x[5])
if OUTF: out.close()
c=collections.Counter(x[1] for x in R)
print('gate: %d rows %s in %.1fs (%.2fs/row wall, NJ=%s)'%(len(R),dict(c),time.time()-t0,(time.time()-t0)/max(1,len(R)),os.environ.get('NJ','4')),file=sys.stderr)
