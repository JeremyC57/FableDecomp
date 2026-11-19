# od.py <src.cpp> [flags...] : compile at /Od (or given flags) and list every function's size and instruction count
import sys,os,subprocess,re
sys.path.insert(0,"/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from pathlib import Path
src=Path(sys.argv[1]); flags=sys.argv[2:] or ["/Od"]
work=Path(os.environ["W"])/"tmp"/"od"; work.mkdir(parents=True,exist_ok=True)
cp=work/"x.cpp"; cp.write_text(V.vc71(src.read_text()))
obj=work/"x.obj"; obj.unlink(missing_ok=True)
e=V.env()
r=V.cl(["/nologo","/c","/EHsc"]+flags+[f"/Fo{obj}",str(cp)],e)
if not obj.exists(): print(r.stdout[-3000:]); sys.exit(1)
out=subprocess.run(["objdump","-d","-M","intel","-w",str(obj)],capture_output=True,text=True).stdout
cur=None;n=0;b=0
res=[]
for l in out.split("\n"):
    m=re.match(r"^([0-9a-f]+) <(.*)>:",l)
    if m:
        if cur: res.append((cur,n,b))
        cur=m.group(2);n=0;b=0;continue
    mm=re.match(r"^\s+[0-9a-f]+:\t([0-9a-f ]+)\t",l)
    if mm and cur: n+=1; b+=len(mm.group(1).split())
if cur: res.append((cur,n,b))
sz=subprocess.run(["objdump","-h",str(obj)],capture_output=True,text=True).stdout
for name,c,bb in res: print(c,"i",bb,"b",name)
