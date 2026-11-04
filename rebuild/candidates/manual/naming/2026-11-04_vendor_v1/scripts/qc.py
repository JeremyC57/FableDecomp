# qc.py <addr> <src> [flags...] : quick parity (verify_and_land.parity_of) with the overlay header dir; prints status + defined leaf
import sys,os,subprocess,re
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from vcommon import *
os.environ['FABLE_HDR_OVERLAY']=W+'/overlay/rebuild/include'
sys.path.insert(0,ROOT+'/tools/decomp_pipeline'); os.chdir(ROOT)
import verify_and_land as V
from pathlib import Path
cat=catalog(); ent=ps1_entries()
a,src=sys.argv[1:3]; extra=sys.argv[3:]
if not extra and a in ent: extra=[f for f in ent[a].get('CompilerFlags','').split() if f not in ('/O2','/Oy','/W3')]
txt=open(src).read(); leaf=leaf_of(txt)
retail=bytes.fromhex(cat[a][3].strip())
while retail and retail[-1] in (0xCC,0x90): retail=retail[:-1]
work=Path(W)/'qwork'/a; work.mkdir(parents=True,exist_ok=True)
st,built,sec=V.parity_of(V.vc71(txt),a,leaf,retail,work,V.env(),extra=tuple(extra))
o=str(work/(a+'.obj'))
syms=[l.split()[-1] for l in subprocess.run(['objdump','-t',o],capture_output=True,text=True).stdout.splitlines() if '(sec  ' in l or '(sec ' in l]
d=[s for s in syms if leaf and s.startswith(leaf)]
print(a,st,'leaf',leaf,'defines' if leaf in syms else ('prefix:'+','.join(d) if d else 'MISSING'))
