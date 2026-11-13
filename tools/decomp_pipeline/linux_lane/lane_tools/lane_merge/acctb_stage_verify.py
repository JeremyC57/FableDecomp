"""acctb_stage_verify.py <workspace>: re-verify every staged row (byte match + test) against the workspace's final private include tree. Never raises."""
import json,os,re,subprocess,sys,glob
S=os.environ['S']; WS=sys.argv[1].rstrip('/'); INC=WS+'/inc'; bad=0
for f in sorted(glob.glob(WS+'/stage/rows/*.json')):
    a=os.path.basename(f)[:8]
    try:
        m=json.load(open(f)); src=f[:-5]+'.cpp'; tst=f[:-5]+'.test.cpp'; t=open(src).read()
        meta=json.load(open(f'{S}/w22/acctb/{a}/meta.json')); leaf=meta.get('leaf') or re.search(r'^// Leaf:\s*(\S+)',t,re.M).group(1)
        Q=f'{WS}/stage/verify/{a}'; os.makedirs(Q,exist_ok=True); env=dict(os.environ,W=Q,FABLE_HDR_OVERLAY=INC,LANDVERIFY_STRICT_HEADERS='1')
        o=subprocess.run(['python3',S+'/tools/fg.py',a,leaf,src,'--lines','6']+(['--flags']+m['flags'] if m.get('flags') else []),capture_output=True,text=True,env=env,timeout=900).stdout
        g=next((l for l in o.split('\n') if l.startswith(a+' ')),'no grade')
        v=subprocess.run(['python3',S+'/tools/vt.py',a,tst],capture_output=True,text=True,env=env,timeout=900).stdout.strip().split('\n')[-1]
        ok=('MATCH' in g) and ('PASS' in v); bad+=0 if ok else 1
        print(a,'OK' if ok else 'REGRESSED',g.split()[-1] if g.split() else g,v.split()[-1] if v.split() else v,flush=True)
    except Exception as e:
        bad+=1; print(a,'ERROR',type(e).__name__,str(e)[:120],flush=True)
print('verify done; not ok:',bad)
