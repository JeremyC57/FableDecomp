#!/usr/bin/env python3
# sweep.py <addr> <base.cpp> <variants.py> : variants.py defines VARIANTS = [(tag, func(src)->src)]; grades each, prints hunks/difflines
import sys, os, subprocess, importlib.util
W=os.environ["W"]; addr=sys.argv[1]; base=open(sys.argv[2]).read()
spec=importlib.util.spec_from_file_location("v", sys.argv[3]); m=importlib.util.module_from_spec(spec); spec.loader.exec_module(m)
os.makedirs(f"{W}/probe/sw", exist_ok=True)
for tag, fn in m.VARIANTS:
    r=fn(base); hdrs={}
    if isinstance(r, tuple): s, hdrs = r
    else: s = r
    saved={h: open(h).read() for h in hdrs}
    for h,c in hdrs.items(): open(h,"w").write(c)
    p=f"{W}/probe/sw/{addr}_{tag}.cpp"; open(p,"w").write(s)
    out=subprocess.run(["python3", f"{W}/tools/g.py", addr, p, "sweep "+tag], capture_output=True, text=True).stdout
    for h,c in saved.items(): open(h,"w").write(c)
    first=out.splitlines()[0] if out else "?"
    print(f"{tag:30} {first.split()[-3] if 'flags' in first else first}  hunks={out.count('@@')} lines={sum(1 for l in out.splitlines() if l[:2] in ('- ','+ '))}", flush=True)
