"""prep_wf.py <wf-name> <N>: take the next N rows from $S/author_pool_rest.json, make $S/w22/<wf>/<addr>/ for each (draft, rows.tsv),
write $S/w22/<wf>/rows.tsv and args.json (the Workflow args), and print the args JSON. Run oracle_from_exe.py on rows.tsv after."""
import json,shutil,os,glob,re,sys
S=os.environ['S']; wf=sys.argv[1]; N=int(sys.argv[2])
pool=json.load(open(S+'/author_pool_rest.json'))
man={}
for l in open('rebuild/manifest/functions.tsv',encoding='utf-8-sig'):
    c=l.rstrip('\n').split('\t')
    if len(c)>2: man[c[0].lower()]=(c[2],c[1])
res=set()
for f in glob.glob('rebuild/candidates/manual/pending/*/RESERVED.tsv'):
    for l in open(f).read().splitlines()[1:]:
        a=l.split('\t')[0].lower().replace('0x','')
        if re.fullmatch(r'[0-9a-f]{8}',a): res.add(a)
landed=set(l.split('\t',1)[0] for l in open('rebuild/oracles/auto-re-candidates.tsv'))
# The row length is the corrected extent of PARKED_INDEX.tsv (corr=<addr>:<len>, else ext=<len>), never the parked grade string:
# 0x00867B40's `468v465` had been graded against a truncated oracle (landing er).
corr={}
for l in open('rebuild/candidates/manual/PARKED_INDEX.tsv',errors='replace'):
    m=re.search(r'corr=([0-9a-f]{8}):(\d+)',l); e=re.search(r'ext=(\d+)',l); a=l.split('\t',1)[0].lower()
    if m and m.group(1)==a: corr[a]=int(m.group(2))
    elif e: corr.setdefault(a,int(e.group(1)))
pool=[[corr.get(x[1],x[0])]+list(x[1:]) for x in pool]
take=[]; rest=[]
for x in pool:
    t,a,rc,nl,d,p=x
    if len(take)<N and a not in res and a not in landed and os.path.exists(p): take.append(x)
    else: rest.append(x)
json.dump(rest,open(S+'/author_pool_rest.json','w'))
base=f'{S}/w22/{wf}'; os.makedirs(base,exist_ok=True)
rows=['address\tmodule\tname\tlength\tsource']; args=[]
for t,a,rc,nl,d,p in take:
    m,nm=man[a]; W=f'{base}/{a}'; os.makedirs(W+'/differ',exist_ok=True); os.makedirs(W+'/hdr',exist_ok=True)
    shutil.copy(p,W+f'/differ/{a}.cpp'); shutil.copy(p,W+'/draft_original.cpp')
    rows.append(f'{a}\t{m}\t{nm}\t{t}\t{p}'); open(W+'/rows.tsv','w').write(rows[0]+'\n'+rows[-1]+'\n')
    args.append({'addr':a,'module':m,'name':nm,'length':t,'residue_class':rc,'pending':os.path.dirname(os.path.dirname(p))})
open(base+'/rows.tsv','w').write('\n'.join(rows)+'\n')
json.dump({'scratch':S,'wf':wf,'rows':args},open(base+'/args.json','w'))
print(json.dumps({'scratch':S,'wf':wf,'rows':args})); sys.stderr.write(f'pool left {len(rest)}\n')
