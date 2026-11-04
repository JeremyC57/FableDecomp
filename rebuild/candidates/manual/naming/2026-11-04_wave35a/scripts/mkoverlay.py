# mkoverlay.py: write overlay/rebuild/src/compiled + overlay/rebuild/tests for $W/all_ok.tsv; RENAMES.tsv; final payload parts
import sys,os,json,re,shutil
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from lib35 import *
ent=ps1_entries(); cat=catalog()
OV=W+'/overlay'
if os.path.exists(OV): shutil.rmtree(OV)
rows=[l.rstrip('\n').split('\t') for l in open(W+'/all_ok.tsv')]
ren=['address\told_source\tnew_source\told_test\tnew_test\tnew_module\told_module\tcatalog_name\tsymbol\tfamily']
tchg=0
for f,a,d in rows:
    d=json.loads(d); kv=ent[a]
    src=open(W+'/'+f+'/src/'+d['new_src']).read()
    p=OV+'/rebuild/src/compiled/'+d['new_src']; os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'w').write(src)
    t=open(ROOT+'/rebuild/tests/'+kv['TestSource'],encoding='utf-8').read()
    pp=kv.get('PassPattern','')
    short=re.sub(r'\W','',d['qual'].split('::')[-1].split('<')[0]) or 'CModel'
    if d['qual'].startswith('CSpawnedFunc1<'): short='CSpawnedFunc1Model'
    t2=t
    for old in ('CLandscapeBackgroundPatch','CEngineSubPrimitiveText','CThingSwitch'):
        if old in t2:
            parts=t2.split(pp) if pp and pp in t2 else [t2]
            t2=pp.join(x.replace(old,short) for x in parts)
    if t2!=t: tchg+=1
    p=OV+'/rebuild/tests/'+d['new_test']; os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'w').write(t2)
    ren.append('\t'.join([a,'rebuild/src/compiled/'+d['old_src'],'rebuild/src/compiled/'+d['new_src'],'rebuild/tests/'+d['old_test'],'rebuild/tests/'+d['new_test'],d['module'],d['old_module'],cat[a][1],d['sym'],f]))
open(W+'/RENAMES.tsv','w').write('\n'.join(ren)+'\n')
print(len(rows),'rows; tests renamed:',tchg)
