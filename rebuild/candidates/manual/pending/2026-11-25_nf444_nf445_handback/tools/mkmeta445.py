#!/usr/bin/env python3
"""mkmeta445.py: NF445 additions to UNITS / TABLE_CHANGES / CATALOG_CHANGES / RENAMES / SELF_SYMBOLS / HEADERS / NAME_FIXES (idempotent)."""
import os
R='/home/user/FableDecomp'; W=os.path.dirname(os.path.dirname(os.path.abspath(__file__))); B=W+'/batches/nf444_hdr'
FUN=['01216180','0121618b','01216196','012161a1','012161ac','012161b7','012161c2','012161cd']
def addlines(f,lines,key=lambda l:l):
    cur=open(f).read().rstrip('\n').split('\n'); have=set(map(key,cur[1:]))
    for l in lines:
        if key(l) not in have: cur.append(l)
    open(f,'w').write('\n'.join(cur)+'\n')
addlines(B+'/UNITS.tsv',['00c6e660\t00c6e630\tS','00c55270\t00c6e630\t-','00c55680\t00c6e630\t-','00c556e0\t00c6e630\t-']+[f'{a}\t00c6e630\t-' for a in FUN],key=lambda l:l.split('\t')[0])
CR='?CreateRenderer@CSalSw3DVoice@@AAEPAVCSalRender@@W4CHRTFMODE@@@Z'; IV='?Initialize@CSalSw3DVoice@@AAE_NW4CHRTFMODE@@@Z'; IR='?Initialize@CSalRender@@QAEXXZ'; CV='?Create@CSalSw3DVoice@@UAEHPBVCCodaSource@@@Z'
addlines(B+'/TABLE_CHANGES.tsv',[
 f'replace\t?Open@CAudioSource@@QAE_NPAX@Z -> {IV}\t00c55680\t00c556e0\tNF445: SDK SalSw3DVoice.obj private ?Initialize@CSalSw3DVoice (96 B = retail; relocations CreateRenderer + ?Initialize@CSalRender = retail calls 0x00C55270 / 0x00C6E660); caller 0x00C556E0 respelled in this batch',
 f'replace\t?CreateStream@CAudioSource@@QAEPAUCStreamHandle@@PAX@Z -> {CR}\t00c55270\t00c55680\tNF445: SDK private ?CreateRenderer@CSalSw3DVoice (relocation of ?Initialize@CSalSw3DVoice at retail 0x00C55689); caller 0x00C55680 respelled in this batch',
 f'replace\t?Prime@CStreamHandle@@QAEXXZ -> {IR}\t00c6e660\t00c55680\tNF445: SDK SalRender.obj ?Initialize@CSalRender (45 B = retail, between ~CSalRender and ClearFilterHistory); caller 0x00C55680 respelled in this batch'],key=lambda l:l.split('\t')[1])
note='NF445 (R-NF444-1 granted): GameCODA render TU, /Ob1'
addlines(B+'/CATALOG_CHANGES.tsv',[
 '00c6e660\tModule\tCRenderCall_00c6e660\tCSalRender\tGameCODA SDK class (NF445; folded into option S unit 00c6e630)',
 '00c6e660\tCompilerFlags\t-\t/O2 /Oy /W3 /Ob1\tR-NF444-1 (granted): whole GameCODA render TU takes /Ob1 (option S thin row of unit 00c6e630)',
 '00c55270\tModule\tCAudioSource\tCSalSw3DVoice\tGameCODA SDK class (NF445): CSalSw3DVoice::CreateRenderer',
 '00c55680\tModule\tCAudioSource\tCSalSw3DVoice\tGameCODA SDK class (NF445): CSalSw3DVoice::Initialize',
 '00c556e0\tModule\tCAudioSource\tCSalSw3DVoice\tGameCODA SDK class (NF445): CSalSw3DVoice::Create'],key=lambda l:tuple(l.split('\t')[:2]))
ev='two sources: GameCODA 2.10 SDK CODAApi.lib Release object symbol (size = retail, relocations = retail calls) + retail caller / vftable 0x012B8AE0 (row header comment)'
addlines(B+'/RENAMES.tsv',[
 f'00c6e660\tCSalRender\tInitialize\t{IR}\tCRenderCall_00c6e660::Call\t?Call@CRenderCall_00c6e660@@QAEXXZ\t{ev}\trebuild/src/compiled/00/c6/CRenderCall00c6e660_Call_00c6e660.cpp\trebuild/src/compiled/00/c6/CRenderCall00c6e660_Call_00c6e660.cpp\trebuild/tests/00/c6/CRenderCall00c6e660_Call_00c6e660_test.cpp',
 f'00c55270\tCSalSw3DVoice\tCreateRenderer\t{CR}\tCAudioSource::CreateProcessor\t?CreateProcessor@CAudioSource@@QAEPAVCVtable_012beda4@@K@Z\t{ev}\trebuild/src/compiled/00/c5/CAudioSource_CreateProcessor_00c55270.cpp\trebuild/src/compiled/00/c5/CAudioSource_CreateProcessor_00c55270.cpp\trebuild/tests/00/c5/CAudioSource_CreateProcessor_00c55270_test.cpp',
 f'00c55680\tCSalSw3DVoice\tInitialize\t{IV}\tCAudioSource::Open\t?Open@CAudioSource@@QAE_NPAX@Z\t{ev}\trebuild/src/compiled/00/c5/CAudioSource_Open_00c55680.cpp\trebuild/src/compiled/00/c5/CAudioSource_Open_00c55680.cpp\trebuild/tests/00/c5/CAudioSource_Open_00c55680_test.cpp',
 f'00c556e0\tCSalSw3DVoice\tCreate\t{CV}\tCAudioSource::Setup\t?Setup@CAudioSource@@UAEJPAUCVoiceDesc_00c556e0@@@Z\t{ev}\trebuild/src/compiled/00/c5/CAudioSource_Setup_00c556e0.cpp\trebuild/src/compiled/00/c5/CAudioSource_Setup_00c556e0.cpp\trebuild/tests/00/c5/CAudioSource_Setup_00c556e0_test.cpp'],key=lambda l:l.split('\t')[0])
ss={}
for l in open(R+'/rebuild/oracles/self_symbols.tsv',encoding='utf-8',errors='replace'):
    p=l.rstrip('\n').split('\t')
    if p: ss.setdefault(p[0],'|'.join(p[1:4]))
sl=[f'00c6e660\t{IR}\tMATCH\t{IR}\t{ss.get("00c6e660","-")}',f'00c55270\t{CR}\tRELOCATION_MATCH\t{CR}\t{ss.get("00c55270","-")}',
    f'00c55680\t{IV}\tRELOCATION_MATCH\t{IV}\t{ss.get("00c55680","-")}',f'00c556e0\t{CV}\tRELOCATION_MATCH\t{CV}\t{ss.get("00c556e0","-")}']
sl+=[f'{a}\t{CR}\tRELOCATION_MATCH\tUnwind@{a}\t{ss.get(a,"-")}' for a in FUN]
addlines(B+'/SELF_SYMBOLS.tsv',sl,key=lambda l:l.split('\t')[0])
addlines(B+'/HEADERS.tsv',['engine/CSalSw3DVoice.h\tadd-only opt-in FABLE_RETAIL_CSALSW3D_CREATE under PASS23 (see orig/hdr_CSalSw3DVoice.h)\t00c55270,00c55680,00c556e0,'+','.join(FUN),
 'engine/CCoda3DSource.h\tadd-only opt-in FABLE_RETAIL_CCODA3DSOURCE_HRTFMODE (see orig/hdr_CCoda3DSource.h)\t00c556e0'],key=lambda l:l.split('\t')[0])
nf=open(B+'/NAME_FIXES.tsv').read().split('\n'); nf=[l for l in nf if not l.startswith('00c6e660\t')]
open(B+'/NAME_FIXES.tsv','w').write('\n'.join(nf))
print('ok')
