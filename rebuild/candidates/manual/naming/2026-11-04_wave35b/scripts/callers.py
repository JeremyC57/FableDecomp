# callers.py: compile each landed caller of a row (an/coderefs.tsv, LANDED, not MINE) and report which spelling of the callee it emits
import sys,os,json,subprocess,csv
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline'); os.chdir('/home/user/FableDecomp')
import verify_and_land as V
W='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/wave35b'
gen={}
for f in 'tstruct3 lvlalloc lvltstruct frameloop plainctor requal lvlctor entctor minent man'.split():
    for l in open(W+'/gen/'+f+'/gen.tsv'):
        a=l.rstrip('\n').split('\t')
        if a[1]=='OK': gen[a[0]]=json.loads(a[2])
binds={}
for r in csv.DictReader(open('rebuild/oracles/reloc_bindings.tsv'),delimiter='\t'): binds[r['symbol']]=r
e=V.env(); wd=W+'/an/callers'; os.makedirs(wd,exist_ok=True)
out=[]
for l in open(W+'/an/coderefs.tsv'):
    a=l.rstrip('\n').split('\t')
    if a[6]!='LANDED' or a[7]=='MINE': continue
    tgt,caller,src=a[0],a[4],a[8]
    sp=wd+'/'+caller+'.cpp'; ob=wd+'/'+caller+'.obj'
    import glob
    if not os.path.exists('rebuild/src/compiled/'+src):
        c=glob.glob('rebuild/src/compiled/*/*/*_'+caller+'.cpp'); src=os.path.relpath(c[0],'rebuild/src/compiled') if c else src
    open(sp,'w').write(V.vc71(open('rebuild/src/compiled/'+src,errors='replace').read()))
    if os.path.exists(ob): os.remove(ob)
    cmd=([V.WINE] if V.WINE else [])+[str(V.CL_EXE)]+V.tool_args(["/nologo","/c"]+V.BASE_FLAGS+["/Fo"+ob,sp])
    subprocess.run(cmd,capture_output=True,text=True,env=e,timeout=120)
    t=subprocess.run(['objdump','-t',ob],capture_output=True,text=True).stdout
    und=set(x.split()[-1] for x in t.split('\n') if '*UND*' in x or '(sec  0)' in x)
    g=gen[tgt]; new=g['leaf']; old=g['old_sym']
    st='NEW' if new in und else ('OLD' if old in und else 'OTHER')
    oth=[s for s in und if s in binds and binds[s]['target']==tgt]
    out.append((tgt,caller,src,st,new if st=='NEW' else (old if st=='OLD' else ';'.join(oth) or ' | '.join(sorted(und))[:600])))
    print(*out[-1],sep='\t')
open(W+'/an/callers.tsv','w').write(''.join('\t'.join(x)+'\n' for x in out))
