# includers.py <stage dir> [<stage dir> ...] -> prints landed sources/tests that reach a changed (existing) header
import os,re,sys,glob
inc='rebuild/include'
changed=set()
for st in sys.argv[1:]:
    for f in glob.glob(st+'/**/*.h',recursive=True):
        rel=os.path.relpath(f,st)
        if os.path.exists(os.path.join(inc,rel)): changed.add(rel)
R=re.compile(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]',re.M)
def resolve(name,fromdir):
    for base in (fromdir,inc,inc+'/stlport',inc+'/engine'):
        p=os.path.normpath(os.path.join(base,name))
        if os.path.isfile(p) and p.startswith(inc): return os.path.relpath(p,inc)
    return None
hdrs=[os.path.relpath(f,inc) for f in glob.glob(inc+'/**/*.h',recursive=True)+glob.glob(inc+'/**/*.hpp',recursive=True)]
rev={}
for h in hdrs:
    try: t=open(os.path.join(inc,h),errors='replace').read()
    except: continue
    for m in R.findall(t):
        r=resolve(m,os.path.dirname(os.path.join(inc,h)))
        if r: rev.setdefault(r,set()).add(h)
reach=set(changed); stack=list(changed)
while stack:
    h=stack.pop()
    for g in rev.get(h,()):
        if g not in reach: reach.add(g); stack.append(g)
out=[]
for f in glob.glob('rebuild/src/compiled/**/*.cpp',recursive=True)+glob.glob('rebuild/tests/**/*.cpp',recursive=True):
    t=open(f,errors='replace').read()
    for m in R.findall(t):
        r=resolve(m,os.path.dirname(f))
        if r in reach: out.append(f); break
sys.stderr.write(f'changed {len(changed)} reach {len(reach)} files {len(out)}\n')
print('\n'.join(sorted(out)))
