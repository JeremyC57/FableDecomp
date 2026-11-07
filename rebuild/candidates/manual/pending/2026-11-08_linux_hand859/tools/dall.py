# dall.py <src.cpp> [flags...]: compile with the lane flags (+overlay) and print objdump -d of the whole object (all functions)
import sys,os,subprocess
sys.path.insert(0,"/home/user/FableDecomp/tools/decomp_pipeline")
S=os.environ['S']; A=S+'/w22/agent859'; os.environ["FABLE_HDR_OVERLAY"]=A+"/hdr"
import verify_and_land as V
from pathlib import Path
src=sys.argv[1]; fl=sys.argv[2:]
w=Path(S)/'wine'/'q859'/'dall'; w.mkdir(parents=True,exist_ok=True)
sp=w/'d.cpp'; sp.write_text(V.vc71(open(src).read()))
e=V.env()
cp=V.cl(["/nologo","/c"]+V.BASE_FLAGS+fl+[f"/Fo{w/'d.obj'}",str(sp)],e)
if cp.returncode: print(cp.stdout[-3000:]); sys.exit(1)
pr=subprocess.run([V.OBJDUMP,"-d","-r","--no-show-raw-insn",str(w/'d.obj')],capture_output=True,text=True)
import re
out=[]
for l in pr.stdout.splitlines():
    if re.match(r'^[0-9a-f]+ <',l) or re.match(r'^\s*[0-9a-f]+:',l) or 'R_386' in l: out.append(l)
print("\n".join(out))
