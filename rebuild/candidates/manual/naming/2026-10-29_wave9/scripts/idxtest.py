import sys, os, subprocess, time, re
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline')
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
import verify_and_land as V
from rows import rows, ROOT
R=rows(); A=sorted(R)[:int(sys.argv[1])]
out=sys.argv[2]; os.makedirs(out,exist_ok=True)
e=V.env()
srcs=[ROOT+'/rebuild/src/compiled/'+R[a]['Source'] for a in A]
t=time.time()
cmd=['timeout','600',V.WINE,str(V.CL_EXE)]+V.tool_args(['/nologo','/c','/O2','/Oy','/W3','/Fo'+out+'/']+srcs)
r=subprocess.run(cmd,capture_output=True,text=True,errors='replace',env=e)
print(time.time()-t, r.returncode, len(os.listdir(out)))
print(r.stdout[-500:])
