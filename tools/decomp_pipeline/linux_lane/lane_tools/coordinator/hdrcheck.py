# hdrcheck.py <overlay_dir>: for each overlay header, compare cpp output vs repo copy under default macros (and any FABLE_RETAIL_ macro already in the repo copy)
# (2026-11-03) Moved from the session scratchpad into the repo; run from the repo root.
import sys,subprocess,re,os,glob
ov=sys.argv[1]
for h in sorted(glob.glob(ov+'/**/*.h',recursive=True)):
    rel=os.path.relpath(h,ov); repo='rebuild/include/'+rel
    if not os.path.exists(repo): print('NEW   ',rel); continue
    macs=[None]+sorted(set(re.findall(r'FABLE_RETAIL_\w+',open(repo,errors='replace').read())))
    res=[]
    for m in macs:
        args=['cpp','-P','-I','rebuild/include','-D_MSC_VER=1310','-include','stddef.h']+([f'-D{m}'] if m else [])
        a=subprocess.run(args+[repo],capture_output=True,text=True).stdout; b=subprocess.run(args+[h],capture_output=True,text=True).stdout
        norm=lambda s:[re.sub(r'fable_static_assert_\d+','SA',l) for l in s.splitlines() if l.strip()]
        res.append(('default' if not m else m)+('=same' if norm(a)==norm(b) else '=DIFF'))
    print('CHANGED',rel,' '.join(res))
