import json,os,sys
sys.path.insert(0,os.environ['S']+'/w23/t31'); from tests import T, FLAGS
S=os.environ['S']; D='rebuild/candidates/manual/batches/2026-10-14_linux_eh31'+os.environ.get('SUFFIX',''); os.makedirs(D,exist_ok=True)
names={'00c27760':('_unattributed','CPlayback_LogStatus_00c27760','LogStatus'),'00c6a650':('CTCKraken','CTCKraken_00c6a650_ctor','??0CTCKraken'),'00cab990':('CAudioBank','CAudioBank_LoadAsync_00cab990','LoadAsync'),'00c1a230':('PKCDefaultFileManager','CFileManager_00c1a230_dtor','??1CFileManager'),'00c1ac40':('PKCDefaultFileManager','CFileManager_00c1ac40_ctor','??0CFileManager'),'00c2d5e0':('CInstanceManager','CInstanceManager_Assign_00c2d5e0','Assign'),'00c0b200':('CAbilitySoundComponent','CAbilitySoundComponent_00c0b200_ctor','??0CAbilitySoundComponent'),'00c62c70':('CSBAgents','CSBAgents_00c62c70_ctor','??0CSBAgents'),'00c49650':('CCodaFrameStreamer','CCodaFrameStreamer_00c49650_dtor','??1CCodaFrameStreamer'),'00c3ec80':('_unattributed','CAnimationOwner_InitializeAnimationGroup_00c3ec80','InitializeAnimationGroup')}
orc={l.split('\t')[0]:l for l in open(S+'/batch/eh_open_oracle.tsv').read().splitlines()}
auth=[]; rows=['address\tname\tlength\tbytes']
only=os.environ.get('ONLY','').split()
for a,(m,n,lf) in names.items():
    if only and a not in only: continue
    src=open(f'{S}/w23/e_{a[2:]}.cpp').read()
    test='#include <cstdio>\n#include <new>\n'+T[a]+'\nint main()\n{\n    if (!Check()) { std::printf("%s_TEST FAIL\\n"); return 1; }\n    std::printf("%s_TEST PASS\\n");\n    return 0;\n}\n'%(a,a)
    c={'address':a,'module':m,'name':n,'source_cpp':src,'test_cpp':test,'pass_pattern':a+'_TEST PASS','flags':['/EHsc']+FLAGS.get(a,[])}
    if lf: c['leaf']=lf
    auth.append(c); rows.append(orc[a])
    base=f"{D}/{m.strip('_')}_{n}_{a}"; open(base+'.cpp','w').write(src); open(base+'_test.cpp','w').write(test)
json.dump({'result':{'authored':auth}},open(D+'/payload.json','w'),indent=1)
open(D+'/oracle.tsv','w').write('\n'.join(rows)+'\n')
print(len(auth))
