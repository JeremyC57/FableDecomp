#!/usr/bin/env python3
"""grid.py <addr> <base.cpp> <spec.py> [--max N] [--dry]
spec defines AXES = [(axis_name, [(tagpart, note, [(old,new),...]), ...]), ...]  (tagpart '' = base choice, no edits)
Generates the cross product, applies edits in axis order (each old must occur exactly once unless it starts with '>' which
allows replace-all), checks brace pairing, skips tags already in tmp/attempts.log, grades 2 at a time via gr.py."""
import sys,os,subprocess,itertools,re,random,concurrent.futures as cf
A=os.environ['S']+'/w22/agent859'
args=sys.argv[1:]; mx=10**9; dry=False
if '--max' in args: i=args.index('--max'); mx=int(args[i+1]); del args[i:i+2]
if '--dry' in args: dry=True; args.remove('--dry')
addr,base,spec=args[:3]
ns={}; exec(open(spec).read(),ns); AXES=ns['AXES']
src=open(base).read()
done=set()
for l in open(A+'/tmp/attempts.log'):
    f=l.split('\t')
    if len(f)>3 and f[1]==addr: done.add(f[2])
combos=list(itertools.product(*[ax[1] for ax in AXES]))
random.Random(859).shuffle(combos)
jobs=[]; skipped=0
for combo in combos:
    tag='_'.join(c[0] for c in combo if c[0]) or 'base'
    note=' + '.join(c[1] for c in combo if c[1]) or 'grid base'
    if tag in done or (tag=='base' and done): skipped+=1; continue
    t=src; ok=True
    for c in combo:
        for o,n in c[2]:
            if o.startswith('?'):
                t=t.replace(o[1:],n); continue
            if o.startswith('>'):
                o=o[1:]
                if o not in t: print(f'{tag}: missing {o[:50]!r}'); ok=False; break
                t=t.replace(o,n)
            else:
                k=t.count(o)
                if k!=1: print(f'{tag}: count {k} for {o[:60]!r}'); ok=False; break
                t=t.replace(o,n)
        if not ok: break
    if not ok: continue
    code=re.sub(r'//.*','',t)
    if code.count('{')!=code.count('}') or code.count('(')!=code.count(')'): print(f'{tag}: brace/paren mismatch'); continue
    p=f'{A}/probe/{addr}_{tag}.cpp'; open(p,'w').write(t); jobs.append((tag,note,p))
    if len(jobs)>=mx: break
print(f'{len(jobs)} variants to grade ({skipped} already logged)', flush=True)
if dry: sys.exit()
def run(j):
    tag,note,p=j
    try:
        r=subprocess.run(['python3',A+'/bin/gr.py',addr,p,tag,'--note',note,'-q'],capture_output=True,text=True,timeout=420)
        line=[l for l in r.stdout.splitlines() if '\t'+addr+'\t' in l]
        return tag+'\t'+(line[0].split('\t',2)[2][:120] if line else 'ERR '+(r.stdout[-200:]+r.stderr[-200:]).replace('\n',' '))
    except subprocess.TimeoutExpired:
        return tag+'\tTIMEOUT'
res=[]
with cf.ThreadPoolExecutor(2) as ex:
    for r in ex.map(run,jobs): print(r,flush=True); res.append(r)
def mdof(r):
    m=re.search(r'md=(\d+)',r); return int(m.group(1)) if m else (0 if 'MATCH' in r else 999)
res.sort(key=mdof); print('BEST:'); print('\n'.join(res[:5]))
