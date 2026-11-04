# symindex.py OUTDIR [addr...|@file|--all] : compile landed rows with their ps1 flags; for the graded function record
#   INDEX.tsv : addr, status, defined symbol (the emitted leaf), compiled length, sha1 of the bytes with relocation fields masked
#   BIND.tsv  : addr (caller), symbol, retail target, kind   -- every relocation of the graded function resolved against retail
# Incremental: a row whose source+flags+include-tree hash is unchanged since the last run is reused (CACHE.tsv).
import sys,os,struct,subprocess,re,hashlib,json
sys.path.insert(0,'/home/user/FableDecomp/rebuild/candidates/manual/naming/2026-11-03_wave33/scripts')
from common import ps1_entries, catalog, ROOT
sys.path.insert(0,ROOT+'/tools/decomp_pipeline'); os.chdir(ROOT)
import verify_and_land as V, reloc_identity as RI
from concurrent.futures import ThreadPoolExecutor
def _rt(exe,args,e,_t=180):
    cmd=([V.WINE] if V.WINE else [])+[str(exe)]+V.tool_args(args)
    try: return subprocess.run(cmd,capture_output=True,text=True,errors='replace',env=e,timeout=_t)
    except subprocess.TimeoutExpired: return subprocess.CompletedProcess(cmd,124,'timeout','')
V.run_tool=_rt
OUT=sys.argv[1]; args=sys.argv[2:]; os.makedirs(OUT,exist_ok=True)
ent=ps1_entries(); cat=catalog(); E=V.env()
if args==['--all']: addrs=sorted(a for a,e in ent.items() if e.get('Source','').endswith('.cpp') and a in cat)
else:
    addrs=[]
    for a in args: addrs+= [l.split()[0] for l in open(a[1:]) if l.strip()] if a.startswith('@') else [a]
def inc_hash():
    h=hashlib.sha1()
    for d,_,fl in sorted(os.walk('rebuild/include')):
        for f in sorted(fl): h.update(f.encode()); h.update(open(os.path.join(d,f),'rb').read())
    return h.hexdigest()
IH=inc_hash()
cache={}
if os.path.exists(OUT+'/CACHE.json'): cache=json.load(open(OUT+'/CACHE.json'))
CC=OUT+'/cc'
def key(a):
    e=ent[a]; p=ROOT+'/rebuild/src/compiled/'+e['Source']
    return hashlib.sha1((open(p,'rb').read().decode('latin1')+'|'+(e.get('CompilerFlags') or '')+'|'+IH).encode('latin1')).hexdigest()
def one(a):
    try:
        k=key(a)
        if a in cache and cache[a]['key']==k: return cache[a]
        e=ent[a]; p=ROOT+'/rebuild/src/compiled/'+e['Source']
        txt=open(p,errors='replace').read(); m=re.search(r'^//\s*Leaf:\s*(\S+)',txt,re.M); leaf=m.group(1) if m else None
        d=CC+'/'+a; os.makedirs(d,exist_ok=True)
        fl=(e.get('CompilerFlags') or '/O2 /Oy /W3').split()
        r=V.cl(['/nologo','/c']+fl+['/Fo'+d+'/x.obj',p],E)
        if r.returncode: return {'key':k,'status':'COMPILE_FAIL','sym':'','len':0,'sha':'','binds':[]}
        q=cat[a]; ln=int(q[2]); b=bytes.fromhex(q[3].strip()); base=int(a,16)
        try: fb,sec,sym=V.obj_text(d+'/x.obj',leaf)
        except Exception: return {'key':k,'status':'NO_FUNC','sym':'','len':0,'sha':'','binds':[]}
        rel=RI.function_relocs('objdump',d+'/x.obj',leaf,ln) or []
        mb=bytearray(fb); binds=[]
        for off,kind,s in rel:
            if 0<=off and off+4<=len(mb): mb[off:off+4]=b'\0\0\0\0'
            if off<0 or off+4>len(b) or s.startswith(RI.SKIP_PREFIXES): continue
            t=(base+off+4+struct.unpack_from('<i',b,off)[0])&0xffffffff if kind in ('REL32','DISP32') else struct.unpack_from('<I',b,off)[0]
            binds.append([s,'%08x'%t,kind])
        return {'key':k,'status':'OK' if (not leaf or leaf in sym) else 'LEAFGUESS','sym':sym,'len':len(fb),'sha':hashlib.sha1(bytes(mb)).hexdigest(),'binds':binds}
    except Exception as ex:
        return {'key':'','status':'ERR '+str(ex)[:120],'sym':'','len':0,'sha':'','binds':[]}
NJ=int(os.environ.get('NJ','4')); done=0
with ThreadPoolExecutor(NJ) as ex:
    for a,res in zip(addrs,ex.map(one,addrs)):
        cache[a]=res; done+=1
        if done%500==0:
            json.dump(cache,open(OUT+'/CACHE.json','w')); print('progress',done,len(addrs),flush=True)
json.dump(cache,open(OUT+'/CACHE.json','w'))
with open(OUT+'/INDEX.tsv','w') as o, open(OUT+'/BIND.tsv','w') as ob:
    o.write('address\tstatus\tsymbol\tlength\tsha1_masked\n'); ob.write('caller\tsymbol\ttarget\tkind\n')
    for a in sorted(cache):
        r=cache[a]; o.write('\t'.join([a,r['status'],r['sym'],str(r['len']),r['sha']])+'\n')
        for s,t,k in r['binds']: ob.write('\t'.join([a,s,t,k])+'\n')
import collections; print(collections.Counter(cache[a]['status'].split()[0] for a in addrs))
