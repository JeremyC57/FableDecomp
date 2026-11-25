#!/usr/bin/env python3
"""mkbatch445.py: add NF445's 12 closure rows to batches/nf444_hdr (payload, oracle, rows.txt, UNITS, TABLE_CHANGES, CATALOG_CHANGES,
RENAMES, SELF_SYMBOLS, HEADERS, orig/ HEAD copies, funclet tests) and refresh the 16 NF444 entries' tests / owner source."""
import json,os,shutil
R='/home/user/FableDecomp'; W=os.path.dirname(os.path.dirname(os.path.abspath(__file__))); B=W+'/batches/nf444_hdr'
FUN=['01216180','0121618b','01216196','012161a1','012161ac','012161b7','012161c2','012161cd']
NEW={ # addr: (module, name, leaf, flags, head_src, head_test)
 '00c556e0':('CSalSw3DVoice','Create','?Create@CSalSw3DVoice@@UAEHPBVCCodaSource@@@Z',[],'00/c5/CAudioSource_Setup_00c556e0.cpp','00/c5/CAudioSource_Setup_00c556e0_test.cpp'),
 '00c55680':('CSalSw3DVoice','Initialize','?Initialize@CSalSw3DVoice@@AAE_NW4CHRTFMODE@@@Z',[],'00/c5/CAudioSource_Open_00c55680.cpp','00/c5/CAudioSource_Open_00c55680_test.cpp'),
 '00c55270':('CSalSw3DVoice','CreateRenderer','?CreateRenderer@CSalSw3DVoice@@AAEPAVCSalRender@@W4CHRTFMODE@@@Z',['/EHsc'],'00/c5/CAudioSource_CreateProcessor_00c55270.cpp','00/c5/CAudioSource_CreateProcessor_00c55270_test.cpp'),
 '00c6e660':('CSalRender','Initialize','?Initialize@CSalRender@@QAEXXZ',['/Ob1'],'00/c6/CRenderCall00c6e660_Call_00c6e660.cpp','00/c6/CRenderCall00c6e660_Call_00c6e660_test.cpp'),
}
OLD={'00c556e0':('CAudioSource','CAudioSource::Setup','?Setup@CAudioSource@@UAEJPAUCVoiceDesc_00c556e0@@@Z'),
     '00c55680':('CAudioSource','CAudioSource::Open','?Open@CAudioSource@@QAE_NPAX@Z'),
     '00c55270':('CAudioSource','CAudioSource::CreateProcessor','?CreateProcessor@CAudioSource@@QAEPAVCVtable_012beda4@@K@Z'),
     '00c6e660':('CRenderCall_00c6e660','CRenderCall_00c6e660::Call','?Call@CRenderCall_00c6e660@@QAEXXZ')}
for i,a in enumerate(FUN):
    NEW[a]=('_global','Unwind@'+a,'Unwind@'+a,['/EHsc'],f'01/21/global_Unwind{a}_{a}.cpp',f'01/21/global_Unwind{a}_{a}_test.cpp')
OFF={a:11*i for i,a in enumerate(FUN)}
# funclet tests: the parent's linked test, renamed
pt=open(B+'/tests/00c55270_test.cpp').read()
for a in FUN:
    t=pt.replace('// Behaviour test of CSalSw3DVoice::CreateRenderer (0x00C55270)',f'// Behaviour test of EH funclet 0x{a.upper()} (unwind state of CSalSw3DVoice::CreateRenderer 0x00C55270; the row object is the\n// parent\'s object). The parent\'s test').replace('00c55270_TEST','%s_TEST'%a)
    open(f'{B}/tests/{a}_test.cpp','w').write(t)
# oracle
orc={}
for l in open(R+'/rebuild/oracles/auto-re-candidates.tsv'):
    p=l.rstrip('\n').split('\t')
    if p[0] in NEW: orc[p[0]]=p
ol=open(B+'/head/oracle.tsv').read().rstrip('\n').split('\n'); have={l.split('\t')[0] for l in ol}
for a in NEW:
    if a not in have: ol.append('\t'.join([a,orc[a][1],orc[a][2],orc[a][3]]))
open(B+'/head/oracle.tsv','w').write('\n'.join(ol)+'\n')
# payload
P=json.load(open(B+'/head/payload.json')); L=P['result']['authored']
L[:]=[e for e in L if e['address'] not in NEW]
for e in L:   # refresh sources / tests of the NF444 rows from rows/ and tests/
    a=e['address']; e['source_cpp']=open(f'{B}/rows/{a}.cpp').read()
    if os.path.exists(f'{B}/tests/{a}_test.cpp'): e['test_cpp']=open(f'{B}/tests/{a}_test.cpp').read()
for a,(m,n,leaf,fl,hs,ht) in NEW.items():
    e={'address':a,'module':m,'name':n,'source_cpp':open(f'{B}/rows/{a}.cpp').read(),'test_cpp':open(f'{B}/tests/{a}_test.cpp').read(),
       'pass_pattern':a+'_TEST PASS','flags':fl,'leaf':leaf}
    if a in OFF: e['funclet_offset']=OFF[a]
    L.append(e)
json.dump(P,open(B+'/head/payload.json','w'),indent=1)
# rows.txt
rows=[l for l in open(B+'/rows.txt').read().split() if l]
for a in NEW:
    if a not in rows: rows.append(a)
open(B+'/rows.txt','w').write('\n'.join(rows)+'\n')
# orig/ HEAD copies
for a,(m,n,leaf,fl,hs,ht) in NEW.items():
    for rel,base in ((hs,'src/compiled'),(ht,'tests')):
        src=f'{R}/rebuild/{base}/{rel}'
        if os.path.exists(src): shutil.copy(src,B+'/orig/'+os.path.basename(rel))
print('rows',len(rows),'payload',len(L),'oracle',len(ol)-1)
