import sys,os,re,json,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from lib35 import *
ent=ps1_entries(); cat=catalog()
rows=[l.rstrip('\n').split('\t') for l in open(W+'/ROWS.tsv')][1:]
for a,cn,leaf,src in rows:
    if not cn.endswith('::OnPersist') or int(cat[a][2])<=3: continue
    s=open(ROOT+'/rebuild/src/compiled/'+src,errors='replace').read()
    calls=re.findall(r'Transfer\("(\w+)",\s*([\w.>-]+),\s*([^)]*)\)',s)
    print(a,cat[a][2],cn.split('::',1)[1],[(k,dv) for k,m,dv in calls], 'CALLS:'+','.join(sorted(set(re.findall(r'0x00[0-9A-Fa-f]{6}',s)))))
