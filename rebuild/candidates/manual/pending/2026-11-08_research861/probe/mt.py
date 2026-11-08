#!/usr/bin/env python3
"""mt.py <src.cpp> [flags...] : compile a micro-TU with the lane cl (BASE_FLAGS + flags) and print the disassembly of
every function in .text (name, then instructions). Usage: probe tool, no oracle."""
import sys, os, re, subprocess
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from pathlib import Path
S=Path(os.environ['S']); A=S/'w22'/'research861'
src=Path(sys.argv[1]); fl=sys.argv[2:]
w=S/'wine'/'t861'/('mt_'+src.stem); w.mkdir(parents=True,exist_ok=True)
sp=w/src.name; sp.write_text(V.vc71(src.read_text()))
obj=w/(src.stem+'.obj'); obj.unlink(missing_ok=True)
e=V.env(); cp=V.cl(["/nologo","/c"]+V.BASE_FLAGS+fl+[f"/Fo{obj}",str(sp)],e)
if cp.returncode or not obj.exists(): print(cp.stdout[-3000:]); sys.exit(1)
pr=subprocess.run(["objdump","-d","-z","-Mintel",str(obj)],capture_output=True,text=True).stdout
cur=None
for l in pr.split('\n'):
    m=re.match(r'^[0-9a-f]+ <(.+)>:$',l)
    if m: cur=m.group(1); print('\n##',cur); continue
    m=re.match(r'^\s*([0-9a-f]+):\s+((?:[0-9a-f]{2} )+)\s*(.*)$',l)
    if m and cur: print(f'  {m.group(1)}: {m.group(3)}')
