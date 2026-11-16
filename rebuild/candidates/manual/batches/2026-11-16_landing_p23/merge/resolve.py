# resolve.py: resolve the diff3 conflict hunks of the pass-23 header merge (p23m/merged), by rule plus three explicit cases.
import os,re,json
M=os.environ['S']+'/p23m/merged/'
special={
 ('engine/CMatrix3x4.h',0): 'ours_else_codex_line',
 ('engine/CThingAICreature.h',0): 'ours',
 ('libpk/CSoundEventManager.h',1): 'csem_tail',
}
log=[]
for f in json.load(open(os.environ['S']+'/p23m/merge_rc.json')):
    p=M+f; t=open(p,encoding='utf-8').read()
    if '<<<<<<< ours' not in t: continue
    out=[];i=0;k=0
    L=t.split('\n')
    while i<len(L):
        if L[i].startswith('<<<<<<< ours'):
            j=L.index('||||||| base',i); m=L.index('=======',j); e=L.index('>>>>>>> codex',m)
            ours,base,cod=L[i+1:j],L[j+1:m],L[m+1:e]
            how=special.get((f,k))
            if how=='ours': res=ours
            elif how=='ours_else_codex_line':
                assert ours[-1].strip()=='FABLE_FORCEINLINE void SetInverse(const CMatrix3x4& m) {' and len(cod)==1
                res=ours[:-1]+cod
            elif how=='csem_tail':
                res=['FABLE_STATIC_ASSERT_AT(12, offsetof(CSoundEventManager,m_pool)==0x360);','#ifdef FABLE_RETAIL_LIBPK_CSOUNDEVENTMANAGER_FADER',
                     'FABLE_STATIC_ASSERT_AT(24, offsetof(CSoundEventManager,m_7b0)==0x7b0);','FABLE_STATIC_ASSERT_AT(25, offsetof(CSoundEventManager,m_fader)==0x7d8);',
                     '#endif','#endif // STREAMED_PASS23']
            else:
                assert not [x for x in base if x.strip()],(f,k,'base not empty')
                res=ours+cod; how='concat'
            log.append((f,k,how)); out+=res; k+=1; i=e+1
        else: out.append(L[i]); i+=1
    open(p,'w',encoding='utf-8').write('\n'.join(out))
for x in log: print(*x)
