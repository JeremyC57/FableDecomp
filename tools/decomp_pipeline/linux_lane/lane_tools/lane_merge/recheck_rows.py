"""recheck_rows.py <addr-file> <outdir> [shards=3]: real VC7.1 recheck (byte match + behaviour test) of landed rows, in parallel shards.
Writes <outdir>/good.txt, <outdir>/bad.tsv (addr, verdict line). Never raises."""
import os,re,subprocess,sys
from concurrent.futures import ThreadPoolExecutor
S=os.environ['S']; L='tools/decomp_pipeline/linux_lane/lane_tools'
addrs=sorted(set(open(sys.argv[1]).read().split())); out=sys.argv[2]; shards=int(sys.argv[3]) if len(sys.argv)>3 else 3
os.makedirs(out,exist_ok=True); CH=60
chunks=[addrs[i:i+CH] for i in range(0,len(addrs),CH)]
def run(k):
    try:
        ch=chunks[k]; Q=f'{out}/c{k:03d}'; os.makedirs(Q,exist_ok=True); open(Q+'/extra.txt','w').write('\n'.join(ch)+'\n')
        subprocess.run(['python3',S+'/mk_recheck_extra.py',Q+'/rc'],env=dict(os.environ,EXTRA=Q+'/extra.txt'),capture_output=True,text=True)
        env=dict(os.environ,LANDVERIFY_WORK=Q+'/work',LANDVERIFY_STRICT_HEADERS='0'); env.pop('FABLE_HDR_OVERLAY',None)
        p=subprocess.run(['python3',L+'/vland.py',Q+'/rc_payload.json',Q+'/rc_oracle.tsv','--recheck-landed'],env=env,capture_output=True,text=True,errors='replace',timeout=7200)
        o=(p.stdout or '')+(p.stderr or ''); open(Q+'/log.txt','w').write(o)
        good=set(re.findall(r'^\s+([0-9a-f]{8}) (?:RELOCATION_MATCH|MATCH) ',o,re.M)); res=[]
        for a in ch:
            if a in good: res.append((a,'OK',''))
            else:
                l=next((x.strip() for x in o.split('\n') if x.strip().startswith(a) and ('PASS' in x or 'FAIL' in x or 'DIFFER' in x or 'SRC' in x)),'no verdict line')
                res.append((a,'BAD',l[:200]))
        return res
    except Exception as e: return [(a,'BAD',f'runner {type(e).__name__}') for a in chunks[k]]
g=open(out+'/good.txt','w'); b=open(out+'/bad.tsv','w'); n=0
with ThreadPoolExecutor(shards) as ex:
    for res in ex.map(run,range(len(chunks))):
        for a,st,l in res:
            n+=1
            if st=='OK': g.write(a+'\n')
            else: b.write(f'{a}\t{l}\n')
        g.flush(); b.flush(); print('rechecked',n,'of',len(addrs),flush=True)
print('recheck done')
