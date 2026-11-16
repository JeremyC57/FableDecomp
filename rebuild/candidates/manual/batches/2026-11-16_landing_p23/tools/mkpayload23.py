# mkpayload23.py: pass-23 landing payload + oracle from the Codex archive (codex_x). Writes payload.json, oracle.tsv, plan.tsv.
import json,csv,re,sys,os
sys.path.insert(0,'tools/decomp_pipeline'); from rebound_landed import retail_bytes
S=os.environ['S']; X=S+'/codex_x'; OUT=S+'/p23l'
EXE=open(S+'/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
exec(open('rebuild/candidates/manual/batches/2026-11-15_landing_gb/merge_tools/mkpayload.py').read().split('au=[]')[0].split('def demangle')[1].join(['def demangle','']) if False else '')
src_gb=open('rebuild/candidates/manual/batches/2026-11-15_landing_gb/merge_tools/mkpayload.py').read()
exec(src_gb[src_gb.index('def demangle'):src_gb.index('au=[]')])
NAMING_BLOCKED={'01210e95','01212472','00c0af90','00c1c460'}
rows=list(csv.DictReader(open(X+'/STATUS.tsv'),delimiter='\t'))
match=[r for r in rows if r['status']=='RELOCATION_MATCH']
def text(a):
    s=open(X+'/rows/%s/%s.cpp'%(a,a),encoding='utf-8').read()
    def inl(m):
        p=X+'/rows/%s/%s'%(a,m.group(1)); t=open(p,encoding='utf-8').read()
        return '// ---- %s (the parent TU, inlined from the Codex row folder) ----\n%s\n// ---- end %s ----'%(m.group(1),t.rstrip('\n'),m.group(1))
    pat=r'^#include "((?:parent_)?[0-9a-f]{8}\.cpp)"'
    while re.search(pat,s,re.M): s=re.sub(pat,inl,s,flags=re.M)
    assert not re.search(r'#include "[^"]*\.cpp"',s),a
    return s
def test(a):
    t=open(X+'/rows/%s/%s_test.cpp'%(a,a),encoding='utf-8').read()
    m=re.findall(r'"([0-9A-Za-z]+_TEST PASS)',t); assert m,a
    return t,m[-1]
FL={r['address'][2:].lower():r['flags'].split() for r in csv.DictReader(open(X+'/STATUS.tsv'),delimiter='\t')}
def flags(a):
    f=FL[a]; s=open(X+'/rows/%s/%s.cpp'%(a,a),encoding='utf-8').read(); m=re.search(r'^// Flags:(.*)$',s,re.M)
    sf=[x for x in (m.group(1).split() if m else []) if x!='none']
    if sorted(sf)!=sorted(f): print('FLAGS DIFFER',a,'status',f,'source',sf)
    return f
VP={'00ca97a0':('?Disconnect@CDynamicOutputPin@@UAGJXZ',127),'00caa860':('?SetSyncSource@CBaseMediaFilter@@UAGJPAUIReferenceClock@@@Z',109),
    '00caa8d0':('?GetSyncSource@CBaseMediaFilter@@UAGJPAPAUIReferenceClock@@@Z',130),'00caa9c0':('?Run@CBaseMediaFilter@@UAGJ_J@Z',148)}
def parent_text(a,rs,psym,mod,nm):
    s=open(X+'/rows/%s/%s.cpp'%(a,a),encoding='utf-8').read()
    m=re.search(r'^#include "((?:parent_)?%s\.cpp)"'%rs,s,re.M)
    if m:
        t=open(X+'/rows/%s/%s'%(a,m.group(1)),encoding='utf-8').read()
        pat=r'^#include "((?:parent_)?[0-9a-f]{8}\.cpp)"'
        def inl(mm):
            q=open(X+'/rows/%s/%s'%(a,mm.group(1)),encoding='utf-8').read()
            return '// ---- %s (inlined from the Codex row folder) ----\n%s\n// ---- end %s ----'%(mm.group(1),q.rstrip('\n'),mm.group(1))
        while re.search(pat,t,re.M): t=re.sub(pat,inl,t,flags=re.M)
    else:
        L=s.split('\n'); assert L[0].startswith('// '+a),(a,L[0])
        L[0]='// %s %s::%s (EH parent; its generated cleanup 0x%s lands as a separate funclet row that includes this file)'%(rs,mod,nm,a.upper()); t='\n'.join(L)
    if not t.startswith('// '+rs) and not t.startswith('// %s'%mod) : t='// %s %s::%s\n'%(rs,mod,nm)+t
    assert '#include "' not in ''.join(x for x in t.split('\n') if re.match(r'#include "[^"]*\.cpp"',x)),rs
    if rs in VP:
        sym,n=VP[rs]; L=t.split('\n')
        L.insert(1,'// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) %s -- %d bytes verified against Fable.exe 0x%s'%(sym,n,rs.upper()))
        t='\n'.join(L)
    return t
au=[];ora=[];plan=[]
landed_parents={'00c3dfd0','00c2e610'}
for r in match:
    a=r['address'][2:].lower(); rs=r['real_start'][2:].lower(); name=r['assigned_name']
    if a in NAMING_BLOCKED: plan.append((a,'HOLD','naming-blocked placeholder names')); continue
    if a in ('004a1c41','008f92ef','00c29a30'): plan.append((a,'BOOK','inside parent %s extent; lands with it'%rs)); 
    if a.startswith('0121'):
        t,pp=test(a); src=text(a)
        e={'address':a,'source_cpp':src,'test_cpp':t,'pass_pattern':pp,'flags':flags(a),'funclet_offset':0,'module':'_global','name':'Unwind'+a}
        e['leaf']=re.search(r'^// Leaf: *(\S+)',src,re.M).group(1)
        au.append(e); ora.append((a,'_global::Unwind'+a,int(r['assigned_span_bytes']))); plan.append((a,'FUNCLET','parent '+rs))
        if rs not in landed_parents and int(r['verified_unique_function_bytes'] or 0)>0 and rs not in NAMING_BLOCKED:
            d=json.load(open(X+'/rows/%s/ROW.json'%a)); psym=d.get('real_parent_symbol') or (name if name.startswith('?') else None)
            assert psym and psym.startswith('?'),(a,psym)
            mod,nm=demangle(psym)
            e2={'address':rs,'source_cpp':parent_text(a,rs,psym,mod,nm),'test_cpp':t,'pass_pattern':pp,'flags':flags(a),'module':mod,'name':nm,'leaf':psym}
            au.append(e2); ora.append((rs,mod+'::'+nm,int(r['retail_size']))); plan.append((rs,'PARENT','from '+a))
        continue
    if a in ('004a1c41','00c29a30'): continue
    t,pp=test(a); src=text(a)
    leaf=re.search(r'^// Leaf: *(\S+)',src,re.M).group(1)
    land=rs
    length=int(r['retail_size'])
    if a=='008f92ef': leaf='?Send@LSocket@LHNetworkLib@@QAE_NAAV?$vector@DV?$allocator@D@std@@@std@@ABULAddress@2@1@Z'; length=970
    mod,nm=demangle(leaf)
    au.append({'address':land,'source_cpp':src,'test_cpp':t,'pass_pattern':pp,'flags':flags(a),'module':mod,'name':nm,'leaf':leaf})
    ora.append((land,mod+'::'+nm,length)); plan.append((land,'ROW','reserved as '+a))
json.dump({'result':{'authored':au}},open(OUT+'/payload.json','w'),indent=1)
with open(OUT+'/oracle.tsv','w') as f:
    f.write('address\tname\tlength\tbytes\n')
    for ad,n,ln in ora: f.write('%s\t%s\t%d\t%s\n'%(ad,n,ln,retail_bytes(EXE,int(ad,16),ln).hex()))
with open(OUT+'/plan.tsv','w') as f:
    for p in plan: f.write('\t'.join(p)+'\n')
import collections; print(len(au),'entries',collections.Counter(p[1] for p in plan))
