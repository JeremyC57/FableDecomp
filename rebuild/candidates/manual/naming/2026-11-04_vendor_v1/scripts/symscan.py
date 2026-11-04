# symscan.py <out.tsv> [--src DIR] addr...|@file : compile each landed row (or DIR/<addr>.cpp when present) with its ps1 flags and
# list the (symbol, retail target) relocation pairs of its graded function. Columns: caller, status, symbol, target, kind
import sys,os,struct,subprocess,re
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from vcommon import *
sys.path.insert(0,ROOT+'/tools/decomp_pipeline')
os.chdir(ROOT)
import verify_and_land as V, reloc_identity as RI
def _rt(exe,args,e,_t=120):
    cmd=([V.WINE] if V.WINE else [])+[str(exe)]+V.tool_args(args)
    try: return subprocess.run(cmd,capture_output=True,text=True,errors='replace',env=e,timeout=_t)
    except subprocess.TimeoutExpired: return subprocess.CompletedProcess(cmd,124,'timeout','')
V.run_tool=_rt
from concurrent.futures import ThreadPoolExecutor
args=sys.argv[1:]; out=args.pop(0); srcdir=None
if args and args[0]=='--src': args.pop(0); srcdir=args.pop(0)
addrs=[]
for a in args:
    if a.startswith('@'): addrs+= [l.split()[0] for l in open(a[1:]) if l.strip()]
    else: addrs.append(a)
ent=ps1_entries(); cat=catalog(); E=V.env()
CC=W+'/cc'
def one(a):
    try:
        e=ent[a]; p=srcdir+'/'+a+'.cpp' if srcdir and os.path.exists(srcdir+'/'+a+'.cpp') else ROOT+'/rebuild/src/compiled/'+e['Source']
        txt=open(p,errors='replace').read(); leaf=leaf_of(txt)
        d=CC+'/'+a; os.makedirs(d,exist_ok=True)
        f=d+'/x.cpp'; open(f,'w').write(txt)
        fl=(e.get('CompilerFlags') or '/O2 /Oy /W3').split()
        r=V.cl(['/nologo','/c']+fl+['/Fo'+d+'/x.obj',f],E)
        if r.returncode: return [(a,'COMPILE_FAIL',r.stdout.strip().splitlines()[-1][:200] if r.stdout.strip() else '','','')]
        q=cat[a]; ln=int(q[2]); b=bytes.fromhex(q[3].strip()); base=int(a,16)
        rel=RI.function_relocs('objdump',d+'/x.obj',leaf,ln)
        if rel is None: return [(a,'NO_BLOCK',leaf or '','','')]
        res=[]
        for off,kind,s in rel:
            if off<0 or off+4>len(b) or s.startswith(RI.SKIP_PREFIXES): continue
            t=(base+off+4+struct.unpack_from('<i',b,off)[0])&0xffffffff if kind in ('REL32','DISP32') else struct.unpack_from('<I',b,off)[0]
            res.append((a,'OK',s,'%08x'%t,kind))
        return res or [(a,'OK','','','')]
    except Exception as ex:
        return [(a,'ERR',str(ex)[:200],'','')]
with ThreadPoolExecutor(int(os.environ.get('NJ','4'))) as ex:
    R=list(ex.map(one,addrs))
with open(out,'w') as o:
    for rr in R:
        for r in rr: o.write('\t'.join(r)+'\n')
import collections
print(collections.Counter(r[1] for rr in R for r in rr[:1]))
