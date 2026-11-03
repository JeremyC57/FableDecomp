# mksib.py: sibling batches (b00..) of landed rows whose preprocessed text changes under the mirror; all non-NSCRIPT-only ones
# plus every 40th NSCRIPT-only one (declaration-only change there: CGameScriptInterface::WorldUpdate).
import json,re,os,csv
W='${S}/w22/chunk734/'
os.chdir('/home/user/FableDecomp')
M="C3DBOUNDINGSPHERE C3DVECTOR CAFILE CBULLETTIMEMANAGER CCOMBATMANAGER CDRIVEMANAGER CENVIRONMENT CGAMEPLAYERINTERFACE NSCRIPT CIVCOUNTEDPOINTER CINITBASECLASS CMATRIX3X4 CNAVIGATORMANAGER CPHYSICSMESHOBJECT PLAYER CPLAYERGUI CQUESTMANAGER CREGION CRIGHTHANDEDSET CTCBASE CTHINGMANAGER CWORLD CWORLDMAP CMAINGAMECOMPONENT NRESOURCEDIRECTORYNAMES CUSERPROFILEMANAGER CCHARSTRING".split()
orc={}
for r in csv.DictReader(open('rebuild/oracles/auto-re-candidates.tsv',encoding='utf-8-sig'),delimiter='\t'):
    orc.setdefault(r['address'].lower(),r)
sel=[];k=0
for l in open(W+'sib/changed.txt'):
    f=l.split('\t')[0]; s=open(f,errors='replace').read()
    ms=[m for m in M if re.search(r'#\s*define\s+FABLE_RETAIL_%s\b'%m,s)]
    if ms and set(ms)<=set(['NSCRIPT','CTCBASE','CCHARSTRING']):   # declaration-only / unused-inline changes: sample
        k+=1
        if k%40: continue
    sel.append((f,ms))
items=[];meta=[]
for f,ms in sel:
    base=os.path.basename(f)[:-4]; addr=base[-8:].lower()
    t=f.replace('rebuild/src/compiled','rebuild/tests').replace('.cpp','_test.cpp')
    if not os.path.exists(t) or addr not in orc: meta.append((addr,base,'NO_TEST_OR_ORACLE',ms)); continue
    s=open(f,errors='replace').read()
    m=re.search(r'^// Leaf: (\S+)',s,re.M); leaf=m.group(1) if m else base.split('_')[-2]
    fl=re.search(r'^// Flags: (.*)$',s,re.M); flags=[x for x in (fl.group(1).split() if fl else []) if x.startswith('/')]
    it={'address':addr,'module':base.split('_')[0],'name':base.split('_')[-2],'source_cpp':s,'test_cpp':open(t,errors='replace').read(),
        'pass_pattern':addr+'_TEST PASS','leaf':leaf}
    if flags: it['flags']=flags
    items.append(it); meta.append((addr,base,'OK',ms))
os.makedirs(W+'sib',exist_ok=True)
with open(W+'sib/oracle.tsv','w') as o:
    o.write('address\tname\tlength\tbytes\n')
    for it in items:
        r=orc[it['address']]; o.write('%s\t%s\t%s\t%s\n'%(it['address'],r['name'],r['length'],r['bytes']))
for i in range(0,len(items),4):
    json.dump({'result':{'authored':items[i:i+4]}},open(W+'sib/b%02d.json'%(i//4),'w'))
with open(W+'sib/meta.tsv','w') as o:
    for a,b,st,ms in meta: o.write('%s\t%s\t%s\t%s\n'%(a,b,st,','.join(ms)))
print(len(items),'items',(len(items)+3)//4,'batches;',sum(1 for x in meta if x[2]!='OK'),'skipped')
