# build.py <rsdir> : overlay/ (strmbase definers + automatic caller/definer respells) + RENAMES.tsv + payload batches pay_<n>/
import sys,os,re,json,shutil
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from vcommon import *
ent=ps1_entries(); cat=catalog()
RS=sys.argv[1]; OV=W+'/overlay'
for d in ('rebuild/src','rebuild/tests'):
    if os.path.exists(OV+'/'+d): shutil.rmtree(OV+'/'+d)
P={l.split('\t')[0]:l.rstrip('\n').split('\t') for l in open(W+'/proven.tsv') if l[:2]=='00'}
DESC={'00ca79f0':'the filter\'s pin version (m_PinVersion), returned for IEnumPins staleness checks',
'00ca48f0':'the renderer has exactly one (input) pin: returns 1 (DX9-era renbase.cpp, before the lazy-create variant)',
'00ca4f20':'empty hook called before each sample is rendered (renbase.cpp; PERF build only does work)',
'00ca4f30':'empty hook called after each sample is rendered (renbase.cpp)',
'00ca53a0':'empty override of the wait-start hook (renbase.cpp; PERF build only)',
'00ca53b0':'empty override of the wait-end hook (renbase.cpp; PERF build only)',
'00ca4a50':'base renderer scheduling policy: never draw immediately (S_FALSE)',
'00ca49b0':'called when the filter goes active: nothing to do (NOERROR)',
'00ca49e0':'accept the connection media type: nothing to store (NOERROR)',
'00ca5380':'end of streaming: m_tStreamingStart becomes the elapsed streaming time (timeGetTime() - start)',
'00ca4b60':'under the renderer lock, whether a media sample is held',
'00ca4bf0':'under the renderer lock, release and clear the held media sample',
'00ca55e0':'record the previous frame\'s lateness, then stamp the render start time',
'00ca4b20':'render one sample: S_FALSE for no sample or when not streaming, else OnRenderStart / DoRenderSample / OnRenderEnd',
'00ca49c0':'filter going inactive: reset the position pass-through\'s media times, then clear the pending sample',
'00ca7030':'reset the end-of-stream state: kill the EOS timer (inline ResetEndOfStreamTimer), then under the renderer lock clear m_bEOS / m_bEOSDelivered / m_SignalTime',
'00ca4a70':'cancel any clock advise (Unadvise + SignalTimerFired), reset the render event; S_OK when an advise was live, else S_FALSE',
'00ca4d80':'start streaming under the renderer lock: timeBeginPeriod(1), OnStartStreaming, then deliver EOS or schedule the held sample',
'00ca4990':'end of a flush: reset the position pass-through\'s media times and let the source thread wait again',
'00ca4b90':'under the renderer lock, AddRef and return the held media sample',
'00ca52a0':'reset every quality-management statistic for a new streaming run',
'00ca5400':'accumulate frame lateness and inter-frame statistics (clamped to +-1 s; the first frames ignored)'}
ren=[]; rows=[]
SD=W+'/strm/src'
for f in sorted(os.listdir(SD)):
    a=f[:-4]; body=open(SD+'/'+f).read(); leaf=leaf_of(body)
    import importlib.util as _iu
    _sp=_iu.spec_from_file_location('nmd',ROOT+'/tools/decomp_pipeline/naming/msvc_demangle.py'); M=_iu.module_from_spec(_sp); _sp.loader.exec_module(M)
    d=M.demangle(leaf); cls,meth=d['comps']; params=', '.join(x for x in d['params'] if x!='void')
    e=ent[a]; old=open(ROOT+'/rebuild/src/compiled/'+e['Source'],errors='replace').read()
    oldfirst=old.split('\n')[0].lstrip('/ ').strip(); oldleaf=leaf_of(old) or 'none'
    ev=P[a][2] if a in P else ''
    hdr=['// %s::%s(%s) -- %s (DirectShow BaseClasses)'%(cls,meth,params,DESC[a]),
         '// Leaf: '+leaf,
         '// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %s bytes, thiscall (__fastcall this=ecx).'%(a.upper(),cat[a][2]),
         '// Identity (vendor naming wave V1, proven): '+ev+'.',
         '//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the',
         '//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.',
         '//   Was: %s (Leaf %s, module %s).'%(oldfirst[:200],oldleaf,e['Module'])]
    src='\n'.join(hdr)+'\n'+'\n'.join(l for l in body.split('\n') if not l.startswith('// Leaf:'))
    if e.get('CompilerFlags') and '/EHsc' in e['CompilerFlags']: pass
    dd=os.path.dirname(e['Source']); base='%s_%s_%s'%(cls,meth,a)
    ns=dd+'/'+base+'.cpp'; nt=os.path.dirname(e['TestSource'])+'/'+base+'_test.cpp'
    p=OV+'/rebuild/src/compiled/'+ns; os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'w').write(src)
    t=open(ROOT+'/rebuild/tests/'+e['TestSource'],errors='replace').read()
    p=OV+'/rebuild/tests/'+nt; os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'w').write(t)
    ren.append((a,e['Source'],ns,e['TestSource'],nt,cls))
    rows.append(dict(address=a,module=cls,name=meth,src=ns,test=nt,leaf=leaf,fam='strm'))
for l in open(RS+'/result.tsv'):
    q=l.rstrip('\n').split('\t')
    if q[1]!='OK' or q[0] in [r['address'] for r in rows]: continue
    a=q[0]; e=ent[a]; src=open(RS+'/'+a+'.cpp').read()
    p=OV+'/rebuild/src/compiled/'+e['Source']; os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'w').write(src)
    rows.append(dict(address=a,module=e['Module'],name=cat[a][1].split('::')[-1],src=e['Source'],test=e['TestSource'],leaf=leaf_of(src),fam='auto'))
with open(W+'/RENAMES.tsv','w') as o:
    o.write('address\told_source\tnew_source\told_test\tnew_test\tnew_module\n')
    for r in ren: o.write('\t'.join(r)+'\n')
json.dump(rows,open(W+'/rows.json','w'),indent=0)
# payload batches
BASE=["/O2","/Oy","/W3"]; N=int(os.environ.get('BATCH','130'))
for i in range(0,len(rows),N):
    out=W+'/pay_%d'%(i//N); os.makedirs(out,exist_ok=True); auth=[]; orc=["address\tname\tlength\tbytes"]
    for r in rows[i:i+N]:
        a=r['address']; kv=ent[a]
        src=open(OV+'/rebuild/src/compiled/'+r['src']).read()
        tp=OV+'/rebuild/tests/'+r['test']; tst=open(tp if os.path.exists(tp) else ROOT+'/rebuild/tests/'+r['test'],errors='replace').read()
        e=dict(address=a,module=r['module'],name=r['name'],source_cpp=src,test_cpp=tst,pass_pattern=kv.get('PassPattern',a+'_TEST PASS'))
        if r['leaf']: e['leaf']=r['leaf']
        fl=kv.get('CompilerFlags','').split(); extra=[x for x in fl if x not in BASE] if fl else []
        if fl and '/Oy' not in fl and '/Oy-' not in extra: extra.append('/Oy-')
        if extra: e['flags']=extra
        auth.append(e); orc.append('\t'.join(cat[a][:4]))
    json.dump({"result":{"authored":auth}},open(out+'/payload.json','w')); open(out+'/oracle.tsv','w').write('\n'.join(orc)+'\n')
    print(out,len(auth))
print(len(rows),'rows',len(ren),'renames')
