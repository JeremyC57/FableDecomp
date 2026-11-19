#!/usr/bin/env python3
"""zs.py <addr> <src> [P] : front-end only (/Zs, or /P with P) with a 150 s timeout, to find hangs/errors."""
import sys, os, subprocess
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from pathlib import Path
W=Path(os.environ["W"]); addr=sys.argv[1]; src=Path(sys.argv[2]).read_text(encoding="utf-8")
work=W/"tmp"/"zs"; work.mkdir(parents=True, exist_ok=True)
sp=work/(addr+".cpp"); sp.write_text(V.vc71(src), encoding="utf-8")
e=V.env()
mode = ["/P", f"/Fi{work/(addr+'.i')}"] if len(sys.argv)>3 else ["/Zs"]
try:
    cp=V.run_tool(V.CL_EXE,["/nologo","/c"]+V.BASE_FLAGS+mode+[str(sp)],e) if False else None
    import threading
    res={}
    def go(): res['cp']=V.cl(["/nologo","/c"]+V.BASE_FLAGS+mode+[str(sp)],e)
    t=threading.Thread(target=go,daemon=True); t.start(); t.join(150)
    if t.is_alive(): raise subprocess.TimeoutExpired("cl",150)
    cp=res['cp']
    print("rc",cp.returncode); print(cp.stdout[-3000:]); print(cp.stderr[-1500:])
except subprocess.TimeoutExpired:
    print("TIMEOUT"); print("kill your own cl manually")
