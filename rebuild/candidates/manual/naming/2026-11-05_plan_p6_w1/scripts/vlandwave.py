# vlandwave.py W [addr...|@file] [--before] [--tag T] : vland --recheck-landed (read-only) on the wave's rows. The payload is
# built like lane_tools/recheck.py (catalog name, Leaf line or mangled prefix, ps1 flags, funclet offset) but with the OVERLAY
# source / test text (--before: the repo's landed text, for a parity baseline). Batches of at most 300, at most 2 vland at once,
# each with its own LANDVERIFY_WORK=$P/vw_<tag>_<n> (deleted after). FABLE_HDR_OVERLAY = W/overlay/rebuild/include when present.
# Writes W/logs/vland_<tag><n>.log and merges W/grades.tsv (address, grade, behaviour, warnings, log) [W/grades_before.tsv].
import sys,os,re,json,subprocess,shutil,collections,threading
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__))); import wavelib as L
W=sys.argv[1]; args=sys.argv[2:]; before='--before' in args; tag='v'
if '--tag' in args: i=args.index('--tag'); tag=args[i+1]; del args[i:i+2]
args=[a for a in args if a!='--before']
addrs=[]
for a in args: addrs+=[l.split()[0] for l in open(a[1:]) if l.strip() and not l.startswith('address')] if a.startswith('@') else [a]
if not addrs: addrs=L.wave_rows(W)
OPS={"operator=":"??4","operator==":"??8","operator!=":"??9","operator[]":"??A","operator()":"??R","operator<":"??M","operator new":"??2",
     "operator delete":"??3","operator new[]":"??_U","operator delete[]":"??_V","operator+=":"??Y","operator-=":"??Z","operator->":"??C"}
def mangled_leaf(leafname,mod):
    cls=mod.rsplit("::",1)[-1]
    def head(n): return ("?$"+n.split("<")[0]) if "<" in n else n
    if leafname.startswith("~"): return "??1"+head(leafname[1:])+"@"
    if leafname==cls or (leafname.split("<")[0]==cls.split("<")[0] and mod!="_global"): return "??0"+head(cls)+"@"
    if leafname in OPS and mod!="_global": return OPS[leafname]+head(cls)+"@"
    return leafname
BASE=["/O2","/Oy","/W3"]
def entry(a):
    kv=L.ent()[a]; q=L.cat()[a]; name=q[1]; mod,leafname=name.rsplit("::",1) if "::" in name else ("_global",name)
    src=(L.read(L.ROOT+'/'+L.src_rel(a)) if before else L.overlay_src(W,a))
    t=L.tst_rel(a); tst=(L.read(L.ROOT+'/'+t) if (before and t) else L.overlay_tst(W,a))
    m=re.search(r"^//\s*Leaf:\s*(\S+)",src,re.M)
    e=dict(address=a,module=kv.get("Module",mod),name=leafname,source_cpp=src,test_cpp=tst,pass_pattern=kv.get("PassPattern",f"{a}_TEST PASS"),
           leaf=m.group(1) if m else mangled_leaf(leafname,mod))
    fo=re.search(r"funclet block offset (0x[0-9a-fA-F]+|\d+)",src)
    if fo and "Unwind@" in name: e["funclet_offset"]=int(fo.group(1),0)
    fl=kv.get("CompilerFlags","").split(); extra=[f for f in fl if f not in BASE] if fl else []
    if fl and "/Oy" not in fl and "/Oy-" not in extra: extra.append("/Oy-")
    if extra: e["flags"]=extra
    return e,"\t".join(q[:4])
os.makedirs(W+'/logs',exist_ok=True); hdr=os.path.join(W,'overlay','rebuild','include')
batches=[addrs[i:i+300] for i in range(0,len(addrs),300)]
res={}; lock=threading.Lock()
def run(n,b):
    d=L.P+'/vw_%s_%d'%(tag,n); shutil.rmtree(d,ignore_errors=True); os.makedirs(d)
    pay=[];orc=["address\tname\tlength\tbytes"]
    for a in b: e,o=entry(a); pay.append(e); orc.append(o)
    json.dump({"result":{"authored":pay}},open(d+'/payload.json','w')); open(d+'/oracle.tsv','w').write("\n".join(orc)+"\n")
    env=dict(os.environ,LANDVERIFY_WORK=d+'/work')
    if os.path.isdir(hdr) and not before: env['FABLE_HDR_OVERLAY']=hdr
    else: env.pop('FABLE_HDR_OVERLAY',None)
    log=W+'/logs/vland_%s%s%02d.log'%(tag,'B' if before else '',n)
    with open(log,'w') as lo:
        subprocess.run(['python3','tools/decomp_pipeline/linux_lane/lane_tools/vland.py',d+'/payload.json',d+'/oracle.tsv','--recheck-landed'],
                       stdout=lo,stderr=subprocess.STDOUT,env=env,cwd=L.ROOT)
    warns=collections.defaultdict(list); grade={}
    for l in open(log,errors='replace'):
        m=re.match(r'^([0-9a-f]{8})\s+WARN\s+(.*)$',l.rstrip())
        if m: warns[m.group(1)].append(m.group(2).strip()); continue
        m=re.match(r'^([0-9a-f]{8})\s+(\S+)\s+(PASS|FAIL|TCC_FAIL|LINK_FAIL|-)\s+(.*)$',l.rstrip())
        if m and m.group(2) not in ('WARN',): grade[m.group(1)]=(m.group(2),m.group(3))
    with lock:
        for a in b: g=grade.get(a,('NOGRADE','-')); res[a]=(g[0],g[1],' | '.join(warns.get(a,[])),os.path.relpath(log,W))
    shutil.rmtree(d,ignore_errors=True)
import concurrent.futures as cf
with cf.ThreadPoolExecutor(2) as ex: list(ex.map(lambda nb: run(*nb),enumerate(batches)))
out=W+('/grades_before.tsv' if before else '/grades.tsv'); old={}
for r in L.tsv(out): old[r['address']]=r
with open(out,'w') as o:
    o.write('address\tgrade\tbehaviour\twarnings\tlog\n')
    for a in sorted(set(old)|set(res)):
        r=res[a] if a in res else (old[a]['grade'],old[a]['behaviour'],old[a]['warnings'],old[a]['log'])
        o.write('\t'.join([a]+list(r))+'\n')
c=collections.Counter(res[a][0]+' '+res[a][1] for a in res); print('vlandwave',W,'before' if before else '',dict(c))
