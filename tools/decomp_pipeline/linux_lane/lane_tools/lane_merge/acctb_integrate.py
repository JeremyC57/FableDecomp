"""acctb_integrate.py <group> [--dry]: bring one group's staged work into the repository.
1. every header its private include tree changed since START_COMMIT is 3-way merged into the CURRENT repo header (base = START_COMMIT);
2. gates: no markers, balanced #if, hdrcheck2 (default path unchanged; opt-ins whose meaning changed -> their landed users are rechecked);
3. headers copied in, staged rows landed with the strict verifier, landed users rechecked; on a header-gate failure nothing is changed.
Prints one line per row. Never raises."""
import json,os,re,shutil,subprocess,sys,glob,importlib.util
S=os.environ['S']; g=sys.argv[1]; dry='--dry' in sys.argv; L='tools/decomp_pipeline/linux_lane/lane_tools'
WS=f'{S}/w22/acctb_ws/{g}'; Q=f'{S}/land/int_{g}'; M=Q+'/M'; INC='rebuild/include'
START=open(f'{S}/w22/acctb_ws/START_COMMIT').read().strip()
def sh(cmd,env=None,timeout=6000):
    try:
        p=subprocess.run(cmd,capture_output=True,text=True,errors='replace',env=env,timeout=timeout); return p.returncode,(p.stdout or '')+(p.stderr or '')
    except Exception as e: return 99,f'{type(e).__name__}: {e}'
def main():
    # reuse the resolver of the landing driver without running its main loop
    src=open(S+'/tools/acctb_land_v2.py').read(); src=src[:src.index("landed=lambda")]
    src=src.replace("S=os.environ['S']; run=sys.argv[1]; addrs=sys.argv[2:]","S=os.environ['S']").replace("RQ=f'{S}/land/{run}'; os.makedirs(RQ,exist_ok=True); INC='rebuild/include'","")
    ns={}; exec(src,ns); resolve=ns['resolve']; balanced=ns['balanced']
    if os.path.isdir(Q): shutil.rmtree(Q)
    os.makedirs(M)
    rows=sorted(os.path.basename(f)[:8] for f in glob.glob(WS+'/stage/rows/*.json'))
    landed=set(l.split('\t',1)[0].lower() for l in open('rebuild/oracles/auto-re-candidates.tsv').read().splitlines()[1:])
    rows=[a for a in rows if a not in landed]
    if not rows: print('nothing staged (or all landed)'); return
    changed=[]; notes=[]
    for d,_,fs in os.walk(WS+'/inc'):
        for f in fs:
            h=os.path.relpath(os.path.join(d,f),WS+'/inc'); theirs=open(os.path.join(d,f),errors='replace').read()
            p=subprocess.run(['git','show',f'{START}:{INC}/{h}'],capture_output=True,text=True,errors='replace'); base=p.stdout if p.returncode==0 else None
            if base is not None and theirs==base: continue
            rp=f'{INC}/{h}'; cur=open(rp,errors='replace').read() if os.path.exists(rp) else None
            if cur is None: merged=theirs; notes.append('new:'+h)
            elif cur==theirs: continue
            elif base is None or cur==base: merged=theirs
            else:
                open(Q+'/_b.h','w').write(base); open(Q+'/_t.h','w').write(theirs)
                p=subprocess.run(['git','merge-file','-p','-L','repo','-L','base','-L','x',rp,Q+'/_b.h',Q+'/_t.h'],capture_output=True,text=True,errors='replace')
                merged,n=resolve(p.stdout)
                if n: notes.append(f'conflicts{n}:{h}')
            if merged==cur: continue
            if '<<<<<<<' in merged or not balanced(merged): print('HEADER GATE: unbalanced or markers in',h); return
            os.makedirs(os.path.dirname(f'{M}/{h}'),exist_ok=True); open(f'{M}/{h}','w').write(merged); changed.append(h)
    recheck=set()
    rc,o=sh(['python3',L+'/coordinator/hdrcheck2.py',M]); open(Q+'/hdrcheck.txt','w').write(o)
    for line in o.split('\n'):
        if not line.startswith('CHANGED'): continue
        p=line.split(); h=p[1]
        for tok in p[2:]:
            m=re.match(r'(\w+)=DIFF\[\d+\](\(cpp-err\))?',tok)
            if not m: continue
            if m.group(1)=='default': print('HEADER GATE: the default path changes in',h); return
            rc2,o2=sh(['grep','-rlw',m.group(1),'rebuild/src','rebuild/tests'])
            for x in o2.split('\n'):
                k=re.search(r'_([0-9a-f]{8})(?:_test)?\.cpp$',x)
                if k: recheck.add(k.group(1))
    print(f'group {g}: rows {len(rows)} | headers changed {len(changed)} | landed users to recheck {len(recheck)} | {" ".join(notes)[:600]}')
    if dry: return
    bk=Q+'/bk'; new=[]
    for h in changed:
        rp=f'{INC}/{h}'
        if os.path.exists(rp): os.makedirs(os.path.dirname(f'{bk}/{h}'),exist_ok=True); shutil.copy(rp,f'{bk}/{h}')
        else: new.append(h); os.makedirs(os.path.dirname(rp),exist_ok=True)
        shutil.copy(f'{M}/{h}',rp)
    json.dump({'changed':changed,'new':new},open(Q+'/headers.json','w'))
    def restore():
        for h in changed:
            rp=f'{INC}/{h}'
            if h in new:
                if os.path.exists(rp): os.remove(rp)
            else: shutil.copy(f'{bk}/{h}',rp)
    if recheck:
        open(Q+'/rc_extra.txt','w').write('\n'.join(sorted(recheck))+'\n')
        sh(['python3',S+'/mk_recheck_extra.py',Q+'/rc'],env=dict(os.environ,EXTRA=Q+'/rc_extra.txt'))
        env0=dict(os.environ,LANDVERIFY_WORK=Q+'/rcwork',LANDVERIFY_STRICT_HEADERS='0'); env0.pop('FABLE_HDR_OVERLAY',None)
        rc0,o0=sh(['python3',L+'/vland.py',Q+'/rc_payload.json',Q+'/rc_oracle.tsv','--recheck-landed'],env=env0); open(Q+'/recheck.log','w').write(o0)
        good=set(re.findall(r'^\s+([0-9a-f]{8}) (?:RELOCATION_MATCH|MATCH) ',o0,re.M))
        if not recheck<=good:
            restore(); print('LANDED USERS BREAK after merging the group headers into the repo:',' '.join(sorted(recheck-good)[:20]),'| headers restored, nothing landed | log',Q+'/recheck.log'); return
        print(f'rechecked {len(recheck)} landed users: all match')
    auth=[]; orc=['address\tname\tlength\tbytes']
    for a in rows:
        m=json.load(open(f'{WS}/stage/rows/{a}.json')); r={'address':a,'module':m['module'],'name':m['name'],'source_cpp':open(f'{WS}/stage/rows/{a}.cpp').read(),'test_cpp':open(f'{WS}/stage/rows/{a}.test.cpp').read(),'pass_pattern':a+'_TEST PASS'}
        if m.get('flags'): r['flags']=m['flags']
        auth.append(r); orc.append(open(f'{S}/w22/acctb/{a}/oracle.tsv').read().strip().split('\n')[1])
    json.dump({'result':{'authored':auth}},open(Q+'/payload.json','w'),indent=1); open(Q+'/oracle.tsv','w').write('\n'.join(orc)+'\n')
    sh(['python3',L+'/chunk_pool/fixmove.py',Q+'/payload.json']); sh(['python3',L+'/naming/keypayload.py',Q+'/payload.json'])
    env=dict(os.environ,LANDVERIFY_WORK=Q+'/work',LANDVERIFY_STRICT_HEADERS='1'); env.pop('FABLE_HDR_OVERLAY',None)
    rc,o=sh(['python3',L+'/vland.py',Q+'/payload.json',Q+'/oracle.tsv','--land'],env=env); open(Q+'/land.log','w').write(o)
    landed2=set(l.split('\t',1)[0].lower() for l in open('rebuild/oracles/auto-re-candidates.tsv').read().splitlines()[1:])
    for a in rows:
        l=next((x.strip() for x in o.split('\n') if x.strip().startswith(a)),'')
        print(a,'LANDED' if a in landed2 else 'NOT LANDED',l[:160])
    w=sorted(set(re.findall(r'WARN (RELOC_\w+|NOT_GENUINE)',o))); print('lander warnings:',' '.join(w))
try: main()
except Exception as e: print('integrate stopped:',type(e).__name__,str(e)[:300])
