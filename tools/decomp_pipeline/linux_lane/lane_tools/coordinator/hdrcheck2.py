# hdrcheck2.py <overlay>: like hdrcheck.py, but system headers are stubbed (empty) so cpp never stops early; reports
# the line count compared so a truncated (vacuous) comparison is visible.
import sys,subprocess,re,os,glob
ov=sys.argv[1]
# Empty system-header stubs, created on demand. They go to $HDRCHECK_STUBS, else $S/stubinc when the lane scratch is set
# (keeps agents' runs out of the repo), else the tracked coordinator/stubinc. A stub dir is first in -I, so never mix it
# with real headers.
_repo_stub=os.path.join(os.path.dirname(os.path.abspath(__file__)),'stubinc')
stub=os.environ.get('HDRCHECK_STUBS') or (os.path.join(os.environ['S'],'stubinc') if os.environ.get('S') else _repo_stub)
if stub!=_repo_stub and not os.path.isdir(stub):
    import shutil; shutil.copytree(_repo_stub,stub)   # start from the tracked stubs
def run(args,f):
    while True:
        r=subprocess.run(args+[f],capture_output=True,text=True)
        m=re.search(r"fatal error: ([^:]+): No such file",r.stderr)
        if not m: return r
        p=os.path.join(stub,m.group(1)); os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'w').close()
for h in sorted(glob.glob(ov+'/**/*.h',recursive=True)):
    if '/stubinc/' in h: continue
    rel=os.path.relpath(h,ov); repo='rebuild/include/'+rel
    if not os.path.exists(repo): print('NEW   ',rel); continue
    macs=[None]+sorted(set(re.findall(r'FABLE_RETAIL_\w+',open(repo,errors='replace').read()+open(h,errors='replace').read())))
    res=[]
    for m in macs:
        args=['cpp','-P','-nostdinc','-undef','-D_MSC_VER=1310','-D_M_IX86=600','-D_WIN32','-I',stub,'-I','rebuild/include']+([f'-D{m}'] if m else [])
        ra=run(args,repo); rb=run(args,h)
        norm=lambda s:[re.sub(r'fable_static_assert_\d+','SA',l) for l in s.splitlines() if l.strip()]
        a=norm(ra.stdout); b=norm(rb.stdout)
        err='' if not (ra.returncode or rb.returncode) else '(cpp-err)'
        res.append(('default' if not m else m)+('=same' if a==b else '=DIFF')+f'[{len(a)}]'+err)
    print('CHANGED',rel,' '.join(res))
