"""acctb_land.py <run-name> addr...: land acctB matched rows one at a time, each behind gates.
Per row: merge its header overlay into the CURRENT rebuild/include (3-way against the version the overlay was made from; both-added
conflicts keep both blocks), check (no markers, #if balance, hdrcheck2: nothing changes for the default path or for any macro a landed
file uses), scan the source for forbidden tokens, then copy the headers and run the landing verifier (strict headers). A row that fails
any step is left unlanded with its headers restored, and the reason is logged. Results: $S/land/<run>/results.tsv. Never raises."""
import json,os,re,shutil,subprocess,sys,difflib,csv
S=os.environ['S']; run=sys.argv[1]; addrs=sys.argv[2:]; L='tools/decomp_pipeline/linux_lane/lane_tools'
RQ=f'{S}/land/{run}'; os.makedirs(RQ,exist_ok=True); INC='rebuild/include'
def sh(cmd,env=None,timeout=3000):
    try:
        p=subprocess.run(cmd,shell=isinstance(cmd,str),capture_output=True,text=True,errors='replace',env=env,timeout=timeout); return p.returncode,(p.stdout or '')+(p.stderr or '')
    except Exception as e: return 99,f'{type(e).__name__}: {e}'
def lost(a,b):
    n=0
    for op,a1,a2,b1,b2 in difflib.SequenceMatcher(None,a,b,autojunk=False).get_opcodes():
        if op in('delete','replace'): n+=a2-a1
    return n
def net(lines):
    n=0
    for l in lines:
        s=l.lstrip()
        if re.match(r'#\s*if',s): n+=1
        elif re.match(r'#\s*endif',s): n-=1
    return n
def balanced(text):
    d=0
    for l in text.split('\n'):
        s=l.lstrip()
        if re.match(r'#\s*if',s): d+=1
        elif re.match(r'#\s*endif',s):
            d-=1
            if d<0: return False
    return d==0
def _norm(l): return re.sub(r'\s+',' ',re.sub(r'//.*','',l)).strip()
def resolve(text):
    """Both-added conflicts keep both blocks. Each conflict has a few ways to place the #endif lines the two blocks share with their
    surroundings; the first combination that leaves the whole file balanced is used."""
    import itertools
    segs=[]; L_=text.split('\n'); i=0; buf=[]
    while i<len(L_):
        if L_[i].startswith('<<<<<<< '):
            j=i+1; A=[]
            while not L_[j].startswith('======='): A.append(L_[j]); j+=1
            j+=1; B=[]
            while not L_[j].startswith('>>>>>>> '): B.append(L_[j]); j+=1
            segs.append(buf); buf=[]
            na=[_norm(x) for x in A if _norm(x)]; nb=[_norm(x) for x in B if _norm(x)]
            pool=list(na); redundant=bool(nb)
            for x in nb:
                if x in pool: pool.remove(x)
                else: redundant=False; break
            ma=set(re.findall(r'FABLE_RETAIL_\w+',' '.join(x for x in A if x.lstrip().startswith('#')))); mb=set(re.findall(r'FABLE_RETAIL_\w+',' '.join(x for x in B if x.lstrip().startswith('#'))))
            if redundant or (mb and mb<=ma): opts=[A]
            else:
                kA=max(net(A),0); kB=max(net(B),0); opts=[]
                for pre,mid in ((0,kA),(kB,kA),(kB,0),(0,0),(kA,kB)):
                    c=['#endif']*pre+A+['#endif']*mid+B
                    if c not in opts: opts.append(c)
            segs.append(opts); i=j+1
        else: buf.append(L_[i]); i+=1
    segs.append(buf)
    confl=[k for k in range(len(segs)) if k%2==1]
    if not confl: return text,0
    for combo in itertools.islice(itertools.product(*[segs[k] for k in confl]),512):
        out=[]; it=iter(combo)
        for k,x in enumerate(segs): out+= (next(it) if k%2==1 else x)
        t='\n'.join(out)
        if balanced(t): return t,len(confl)
    out=[]
    for k,x in enumerate(segs): out+= (x[0] if k%2==1 else x)
    return '\n'.join(out),len(confl)
def _blocks(text,M):
    """Texts of the #ifdef M / #if defined(M) ... #endif blocks (simple single-macro conditions only); None if M is used in a compound condition."""
    L_=text.split('\n'); out=[]; i=0
    simple=re.compile(r'^\s*#\s*(?:ifdef\s+%s\s*$|if\s+defined\s*\(?\s*%s\s*\)?\s*$)'%(M,M))
    anyuse=re.compile(r'^\s*#\s*(?:if|ifdef|ifndef|elif)\b.*\b%s\b'%M)
    while i<len(L_):
        if simple.match(re.sub(r'//.*','',L_[i]).rstrip()):
            d=1; j=i+1
            while j<len(L_) and d>0:
                t=L_[j].lstrip()
                if re.match(r'#\s*if',t): d+=1
                elif re.match(r'#\s*endif',t): d-=1
                j+=1
            out.append((i,j,L_[i:j])); i=j
        else:
            if anyuse.match(L_[i]): return None
            i+=1
    return out
def _strip_blocks(text,M):
    bl=_blocks(text,M); L_=text.split('\n')
    for i,j,_ in reversed(bl or []): del L_[i:j]
    return '\n'.join(L_)
def _uses(text,M): return bool(re.search(r'^\s*#\s*(?:if|ifdef|ifndef|elif)\b.*\b%s\b'%M,text,re.M))
landed=lambda: set(l.split('\t',1)[0].lower() for l in open('rebuild/oracles/auto-re-candidates.tsv').read().splitlines()[1:])
users_cache={}
def users(m):
    if m not in users_cache:
        rc,o=sh(['grep','-rlw',m,'rebuild/src','rebuild/tests','rebuild/include']); users_cache[m]=len([x for x in o.split('\n') if x.strip()])
    return users_cache[m]
def one(a):
    W=f'{S}/w22/acctb/{a}'; Q=f'{RQ}/{a}'; M=Q+'/M'
    if os.path.isdir(Q): shutil.rmtree(Q)
    os.makedirs(M); notes=[]
    if a in landed(): return 'ALREADY_LANDED',''
    if not os.path.exists(W+'/payload.json'): return 'NO_ROW',''
    row=json.load(open(W+'/payload.json'))['result']['authored'][0]
    if os.path.exists(W+'/fix/src.cpp'): row['source_cpp']=open(W+'/fix/src.cpp').read(); notes.append('hand:source')
    if os.path.exists(W+'/fix/test.cpp'): row['test_cpp']=open(W+'/fix/test.cpp').read(); notes.append('hand:test')
    def strip(s): return re.sub(r'//[^\n]*','',re.sub(r'/\*.*?\*/','',s,flags=re.S))
    for key in('source_cpp','test_cpp'):
        c=strip(row[key]); bad=[t for t in('__asm','naked','_emit','register ','volatile','__forceinline') if re.search(r'(?<![A-Za-z_])'+re.escape(t),c)]
        if bad: return 'TOKENS',f'{key}: {bad}'
    c=strip(row['source_cpp'])
    for m in re.finditer(r'^\s*(?:class|struct)\s+(?:__declspec\(\w+\)\s+)?(\w+)\s*(?::[^{;]*)?\{',c,re.M):
        if os.path.exists(f'{INC}/engine/{m.group(1)}.h'): notes.append('local-model:'+m.group(1))
    if re.search('stand-in',row['source_cpp'],re.I): notes.append('stand-in-text')
    # header merge
    hs=set(); seen=set(); todo=[]
    def scan(text,here):
        for m in re.finditer(r'^[ \t]*#[ \t]*include[ \t]*["<]([^">]+)[">]',text,re.M):
            inc=m.group(1).replace('\\','/')
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
    changed=[]; recheck=set()
    # Opt-in names both lanes introduced independently: the same content -> drop theirs (the repo has it); different content -> theirs is renamed <M>_B
    ovt={h:open(f'{W}/hdr/{h}',errors='replace').read() for h in hs}; rename={}; strip={}; curx={}
    def _free(h_all,M_,theirs_norm):
        """First of M_B, M_B2, ... that no repo header uses, or uses with the same content."""
        k=1
        while True:
            cand=M_+('_B' if k==1 else f'_B{k}'); clash=False
            for h2 in h_all:
                rp2=f'{INC}/{h2}'
                if not os.path.exists(rp2): continue
                t2=open(rp2,errors='replace').read()
                if not _uses(t2,cand): continue
                b2=_blocks(t2,cand); bo2=_blocks(ovt[h2],M_) if h2 in ovt else []
                if b2 is None or bo2 is None: clash=True; break
                n2=[_norm(x) for _,_,b in b2 for x in b if _norm(x)]; no2=[_norm(x).replace(M_,cand) for _,_,b in bo2 for x in b if _norm(x)]
                if not all(x in n2 for x in no2): clash=True; break
            if not clash: return cand
            k+=1
            if k>9: return cand
    for h in sorted(hs):
        rp=f'{INC}/{h}'
        if not os.path.exists(rp): continue
        cur=open(rp,errors='replace').read()
        if cur==ovt[h]: continue
        sh(['python3',S+'/tools/find_base.py',h,f'{W}/hdr/{h}',Q+'/_base.h']); base=open(Q+'/_base.h',errors='replace').read()
        for M_ in sorted(set(re.findall(r'FABLE_RETAIL_\w+',' '.join(l for l in ovt[h].split('\n') if re.match(r'\s*#\s*(?:if|ifdef|ifndef|elif)\b',l))))):
            if M_ in rename: continue
            if _uses(base,M_) or not _uses(cur,M_): continue
            bo=_blocks(ovt[h],M_); bc=_blocks(cur,M_)
            if bo is None or bc is None or not bo or not bc: rename[M_]=None; continue
            no=[_norm(x) for _,_,b in bo for x in b if _norm(x)]; nc=[_norm(x) for _,_,b in bc for x in b if _norm(x)]
            if all(x in nc for x in no): strip.setdefault(h,[]).append(M_)
            elif all(x in no for x in nc) and len(bo)==len(bc):
                # theirs is the same opt-in with more in it: the repo block takes the incoming text (its users are rechecked below)
                L_=curx.get(h,cur).split('\n'); bc2=_blocks('\n'.join(L_),M_)
                for (i1,j1,_),(_,_,tb) in reversed(list(zip(bc2,bo))): L_[i1:j1]=tb
                curx[h]='\n'.join(L_); strip.setdefault(h,[]).append(M_); notes.append(f'grown-optin:{M_}')
            else: rename[M_]=None
    for M_ in list(rename): rename[M_]=_free(sorted(hs),M_,None)
    for h,ms in strip.items():
        for M_ in ms: ovt[h]=_strip_blocks(ovt[h],M_); notes.append(f'same-optin:{M_}')
    if rename:
        rx=re.compile(r'\b(%s)\b'%'|'.join(map(re.escape,sorted(rename,key=len,reverse=True))))
        for h in ovt: ovt[h]=rx.sub(lambda m:rename[m.group(1)],ovt[h])
        row['source_cpp']=rx.sub(lambda m:rename[m.group(1)],row['source_cpp']); row['test_cpp']=rx.sub(lambda m:rename[m.group(1)],row['test_cpp'])
        notes+= [f'renamed:{k}' for k in sorted(rename)]
    os.makedirs(Q+'/ov',exist_ok=True)
    for h in ovt:
        os.makedirs(os.path.dirname(f'{Q}/ov/{h}'),exist_ok=True); open(f'{Q}/ov/{h}','w').write(ovt[h])
    for h in sorted(hs):
        ov=ovt[h]; rp=f'{INC}/{h}'
        ej=f'{S}/w22/acctb/_fix/{h}.edits.json'      # hand merge by the coordinator: [old, new] replacements on the CURRENT repo header
        if os.path.exists(ej) and os.path.exists(rp):
            cur=open(rp,errors='replace').read(); merged=cur
            for old_,new_ in json.load(open(ej)):
                if new_ in merged: continue           # already applied by an earlier row
                if merged.count(old_)!=1: return 'HAND_EDIT_DOES_NOT_APPLY',h+' | '+old_[:80].replace('\n','\\n')
                merged=merged.replace(old_,new_)
            if merged!=cur:
                os.makedirs(os.path.dirname(f'{M}/{h}'),exist_ok=True); open(f'{M}/{h}','w').write(merged); changed.append(h); notes.append('hand:'+h)
            continue
        if os.path.exists(f'{W}/fix/{h}'):          # hand-merged by the coordinator against the current repo header
            merged=open(f'{W}/fix/{h}',errors='replace').read(); cur=open(rp,errors='replace').read() if os.path.exists(rp) else None
            if merged!=cur:
                if cur is None: notes.append('new:'+h)
                os.makedirs(os.path.dirname(f'{M}/{h}'),exist_ok=True); open(f'{M}/{h}','w').write(merged); changed.append(h); notes.append('hand:'+h)
            continue
        if not os.path.exists(rp):
            os.makedirs(os.path.dirname(f'{M}/{h}'),exist_ok=True); open(f'{M}/{h}','w').write(ov); changed.append(h); notes.append('new:'+h); continue
        cur=open(rp,errors='replace').read(); cur0=cur
        if h in curx: cur=curx[h]; open(Q+'/_cur.h','w').write(cur)
        if cur==ov and cur0==cur: continue
        if lost(cur.split('\n'),ov.split('\n'))==0: merged=ov
        else:
            rc,o=sh(['python3',S+'/tools/find_base.py',h,f'{W}/hdr/{h}',Q+'/_base.h']); m=re.search(r'lost=(\d+)',o)
            if rename:
                bt=open(Q+'/_base.h',errors='replace').read(); open(Q+'/_base.h','w').write(rx.sub(lambda m_:rename[m_.group(1)],bt)) if False else None
            if m and int(m.group(1))>3:
                notes.append(f'kept-repo(independent version, {m.group(1)} lines differ):{h}'); continue   # both lanes wrote this header: the landed one stands
            if m and int(m.group(1))>0: notes.append(f'rewrote{m.group(1)}:{h}')
            p=subprocess.run(['git','merge-file','-p','-L','repo','-L','base','-L','x',(Q+'/_cur.h' if h in curx else rp),Q+'/_base.h',f'{Q}/ov/{h}'],capture_output=True,text=True,errors='replace')
            merged,n=resolve(p.stdout)
            if n: notes.append(f'conflicts{n}:{h}')
        if merged==cur0: continue
        cur=cur0
        if '<<<<<<<' in merged or '>>>>>>>' in merged: return 'HDR_MARKERS',h
        if not balanced(merged): return 'HDR_UNBALANCED',h+' | '+' '.join(notes)
        if lost(cur.split('\n'),merged.split('\n'))>0: notes.append('removes-repo-lines:'+h)
        os.makedirs(os.path.dirname(f'{M}/{h}'),exist_ok=True); open(f'{M}/{h}','w').write(merged); changed.append(h)
    if changed:
        rc,o=sh(['python3',L+'/coordinator/hdrcheck2.py',M]); open(Q+'/hdrcheck.txt','w').write(o)
        stub=os.path.join(S,'stubinc')
        for h in changed:
            p_=subprocess.run(['cpp','-P','-nostdinc','-undef','-D_MSC_VER=1310','-D_M_IX86=600','-D_WIN32','-I',M,'-I','rebuild/include','-I',stub,f'{M}/{h}'],capture_output=True,text=True,errors='replace')
            e=[l for l in p_.stderr.split('\n') if re.search(r'#elif after #else|#else after #else|without #if|unterminated|#endif without',l)]
            if e: return 'HDR_STRUCTURE',h+' | '+e[0][-120:]
        for line in o.split('\n'):
            if not line.startswith('CHANGED'): continue
            p=line.split(); h=p[1]
            for tok in p[2:]:
                m=re.match(r'(\w+)=DIFF\[\d+\](\(cpp-err\))?',tok)
                if not m: continue
                if m.group(1)=='default': return 'HDR_DEFAULT_CHANGES',h+' | '+' '.join(notes)
                rc2,o2=sh(['grep','-rlw',m.group(1),'rebuild/src','rebuild/tests','rebuild/include'])
                fs=[x for x in o2.split('\n') if x.strip() and not x.endswith('/'+h)]
                if not fs: continue
                if m.group(2): notes.append(f'cpp-err-under:{m.group(1)}')
                definers=[x for x in fs if x.startswith('rebuild/include') and re.search(r'^\s*#\s*define\s+%s\b'%m.group(1),open(x,errors='replace').read(),re.M)]
                if definers: return 'HDR_USED_MACRO_DEFINED_BY_HEADER',f'{h} {m.group(1)} defined in {definers[0]}'
                for x in fs:
                    k=re.search(r'_([0-9a-f]{8})(?:_test)?\.cpp$',x)
                    if k: recheck.add(k.group(1))
                notes.append(f'used-macro:{m.group(1)}')
    if len(recheck)>60: return 'HDR_TOO_MANY_USERS',f'{len(recheck)} landed rows use opt-ins this row changes | '+' '.join(notes)
    if os.environ.get('ACCTB_DRY'):
        open(Q+'/src.cpp','w').write(row['source_cpp']); meta=json.load(open(W+'/meta.json'))
        leaf=meta.get('leaf') or (re.search(r'^// Leaf:\s*(\S+)',row['source_cpp'],re.M) or [None,''])[1]; fl=row.get('flags') or []
        envd=dict(os.environ,W=Q,FABLE_HDR_OVERLAY=M,LANDVERIFY_STRICT_HEADERS='1')
        rc,o=sh(['python3',S+'/tools/fg.py',a,leaf,Q+'/src.cpp','--lines',os.environ.get('ACCTB_LINES','14')]+(['--flags']+fl if fl else []),env=envd,timeout=600)
        return 'DRY',o.strip()[:int(os.environ.get('ACCTB_CHARS','1500'))]+' || '+' '.join(notes)+(' || recheck='+','.join(sorted(recheck)) if recheck else '')
    # copy headers (with backup), land
    bk=Q+'/bk'; new=[]
    for h in changed:
        rp=f'{INC}/{h}'
        if os.path.exists(rp): os.makedirs(os.path.dirname(f'{bk}/{h}'),exist_ok=True); shutil.copy(rp,f'{bk}/{h}')
        else: new.append(h); os.makedirs(os.path.dirname(rp),exist_ok=True)
        shutil.copy(f'{M}/{h}',rp)
    def restore():
        for h in changed:
            rp=f'{INC}/{h}'
            if h in new:
                if os.path.exists(rp): os.remove(rp)
            else: shutil.copy(f'{bk}/{h}',rp)
        for m_ in list(users_cache): users_cache.pop(m_)
    if recheck:
        open(Q+'/rc_extra.txt','w').write('\n'.join(sorted(recheck))+'\n')
        sh(['python3',S+'/mk_recheck_extra.py',Q+'/rc'],env=dict(os.environ,EXTRA=Q+'/rc_extra.txt'))
        env0=dict(os.environ,LANDVERIFY_WORK=Q+'/rcwork',LANDVERIFY_STRICT_HEADERS='0'); env0.pop('FABLE_HDR_OVERLAY',None)
        rc0,o0=sh(['python3',L+'/vland.py',Q+'/rc_payload.json',Q+'/rc_oracle.tsv','--recheck-landed'],env=env0); open(Q+'/recheck.log','w').write(o0)
        good=set(re.findall(r'^\s+([0-9a-f]{8}) (?:RELOCATION_MATCH|MATCH) ',o0,re.M))
        if not recheck<=good:
            restore(); return 'LANDED_USERS_BREAK',f'{len(recheck-good)} of {len(recheck)} landed rows stop matching: '+' '.join(sorted(recheck-good)[:6])+' | '+' '.join(notes)
        notes.append(f'rechecked{len(recheck)}')
    json.dump({'result':{'authored':[row]}},open(Q+'/payload.json','w'),indent=1); shutil.copy(W+'/oracle.tsv',Q+'/oracle.tsv')
    sh(['python3',L+'/chunk_pool/fixmove.py',Q+'/payload.json']); sh(['python3',L+'/naming/keypayload.py',Q+'/payload.json'])
    env=dict(os.environ,LANDVERIFY_WORK=Q+'/work',LANDVERIFY_STRICT_HEADERS='1'); env.pop('FABLE_HDR_OVERLAY',None)
    rc,o=sh(['python3',L+'/vland.py',Q+'/payload.json',Q+'/oracle.tsv','--land'],env=env); open(Q+'/land.log','w').write(o)
    ok=re.search(r'^%s\s+(RELOCATION_MATCH|MATCH)\s+PASS'%a,o,re.M); bad=re.findall(r'RELOC_JUMPTABLE|RELOC_IDENTITY|NOT_GENUINE',o)
    warn=sorted(set(re.findall(r'WARN (RELOC_\w+)',o)))
    if a in landed() and ok and not bad:
        users_cache.clear(); return 'LANDED',ok.group(1)+' | hdr='+str(len(changed))+' | '+' '.join(warn+notes)
    restore()
    if a in landed(): return 'LANDED_BUT_FLAGGED',' '.join(bad+warn+notes)
    last=[l for l in o.split('\n') if l.strip().startswith(a)]
    return 'VERIFY_FAIL',(last[-1].strip()[:200] if last else o.strip()[-200:].replace('\n',' '))+' | '+' '.join(notes)
import time
_t0=time.time(); _budget=float(os.environ.get('ACCTB_BUDGET','1e9')); _done=set(l.split('\t')[0] for l in open(RQ+'/results.tsv').read().split('\n') if l) if os.path.exists(RQ+'/results.tsv') else set()
with open(RQ+'/results.tsv','a') as R:
    for a in addrs:
        if a in _done: continue
        if time.time()-_t0>_budget: print('budget reached; rerun to continue'); break
        try: st,info=one(a)
        except Exception as e: st,info='DRIVER_ERROR',f'{type(e).__name__}: {str(e)[:200]}'
        R.write(f'{a}\t{st}\t{info}\n'); R.flush(); print(a,st,info[:300],flush=True)
print('run done')
