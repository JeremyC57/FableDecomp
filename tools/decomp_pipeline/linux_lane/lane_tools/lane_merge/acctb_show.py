"""acctb_show.py <addr> [header-substring]: read-only view of what a row's overlay does to each header it includes: the merge against the
current repo header, with conflict regions and added lines. Writes nothing in the repo."""
import json,os,re,subprocess,sys
S=os.environ['S']; a=sys.argv[1]; sub=sys.argv[2] if len(sys.argv)>2 else ''; W=f'{S}/w22/acctb/{a}'; INC='rebuild/include'
row=json.load(open(W+'/payload.json'))['result']['authored'][0]
hs=set(); seen=set(); todo=[]
def scan(text,here):
    for m in re.finditer(r'^[ \t]*#[ \t]*include[ \t]*["<]([^">]+)[">]',text,re.M):
        inc=m.group(1)
        for cand in ([os.path.normpath(os.path.join(here,inc))] if here is not None else [])+[inc]:
            if cand.startswith('..'): continue
            if os.path.exists(f'{W}/hdr/{cand}') or os.path.exists(f'{INC}/{cand}'):
                if cand not in seen: seen.add(cand); todo.append(cand)
                break
scan(row['source_cpp'],None); scan(row['test_cpp'],None)
while todo:
    h=todo.pop()
    if os.path.exists(f'{W}/hdr/{h}'): hs.add(h); t=open(f'{W}/hdr/{h}',errors='replace').read()
    else: t=open(f'{INC}/{h}',errors='replace').read()
    scan(t,os.path.dirname(h))
os.makedirs(W+'/show',exist_ok=True)
for h in sorted(hs):
    if sub and sub not in h: continue
    rp=f'{INC}/{h}'; ov=f'{W}/hdr/{h}'
    if not os.path.exists(rp): print('NEW',h,len(open(ov).read().split('\n')),'lines'); continue
    if open(rp,errors='replace').read()==open(ov,errors='replace').read(): continue
    b=W+'/show/_base.h'; o=subprocess.run(['python3',S+'/tools/find_base.py',h,ov,b],capture_output=True,text=True).stdout.strip()
    p=subprocess.run(['git','merge-file','-p','-L','repo','-L','base','-L','x',rp,b,ov],capture_output=True,text=True,errors='replace')
    mf=W+'/show/'+h.replace('/','__'); open(mf,'w').write(p.stdout)
    d=subprocess.run(['diff',rp,mf],capture_output=True,text=True,errors='replace').stdout
    add=d.count('\n> '); rem=d.count('\n< ')
    if not add and not rem and not p.returncode: continue
    print(f'=== {h} {o} conflicts={p.returncode} +{add} -{rem}  merged copy: {mf}')
    if sub or p.returncode or rem: print('\n'.join(l[:210] for l in d.split('\n')[:int(os.environ.get("N","80"))]))
