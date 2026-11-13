"""Grade every acctB matched row against its OWN header overlay in this environment; fix the oracle length when the source is shorter. $S/w22/acctb/baseline.tsv"""
import json,os,re,subprocess,csv
from concurrent.futures import ThreadPoolExecutor
S=os.environ['S']; B=S+'/w22/acctb'; o=json.load(open(B+'/order.json'))
def one(a):
    try:
        W=f'{B}/{a}'; r=json.load(open(W+'/payload.json'))['result']['authored'][0]; m=json.load(open(W+'/meta.json'))
        open(W+'/src.cpp','w').write(r['source_cpp'])
        leaf=m.get('leaf') or (re.search(r'^// Leaf:\s*(\S+)',r['source_cpp'],re.M) or [None,None])[1]
        if not leaf: return a,'NO_LEAF',''
        fl=r.get('flags') or []
        cmd=['python3',S+'/tools/fg.py',a,leaf,W+'/src.cpp','--lines','4']+(['--flags']+fl if fl else [])
        env=dict(os.environ,W=W,FABLE_HDR_OVERLAY=W+'/hdr',LANDVERIFY_STRICT_HEADERS='1')
        p=subprocess.run(cmd,capture_output=True,text=True,errors='replace',timeout=500,env=env); t=p.stdout
        l=next((x for x in t.split('\n') if x.startswith(a+' ')),'')
        g=re.search(r'(RELOCATION_MATCH|MATCH|DIFFER\(\d+v\d+\)\s*hunks=\d+|SRC_FAIL|TIMEOUT|ERROR)',l)
        return a,(g.group(1) if g else 'ERROR'),' / '.join(t.split('\n')[1:5])[:300] if not (g and 'MATCH' in g.group(1)) else ''
    except Exception as e: return a,'ERROR',f'{type(e).__name__} {str(e)[:100]}'
open(B+'/baseline.tsv','w').close()
with ThreadPoolExecutor(3) as ex:
    for a,g,info in ex.map(one,o):
        open(B+'/baseline.tsv','a').write(f'{a}\t{g}\t{info}\n')
print('baseline done')
