import sys,os,re,json,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from lib35 import *
ent=ps1_entries(); cat=catalog(); P=pdbmap()
byname={}
for a,q in cat.items(): byname.setdefault(q[1],[]).append(a)
rows=[l.rstrip('\n').split('\t') for l in open(W+'/ROWS.tsv')][1:]
for a,cn,leaf,src in rows:
    m=re.match(r"^NScript::(\w+)::(OnPersist|`scalar deleting destructor')$",cn)
    if not m: continue
    O,M=m.groups()
    ctor=[x for x in byname.get('NScript::%s::%s'%(O,O),[]) if x in ent]
    h=[x for x in ('engine/%s.h'%O,'engine/NScript_%s.h'%O) if os.path.exists(ROOT+'/rebuild/include/'+x)]
    syms=[s for s in P if s.startswith(('?OnPersist@%s@NScript@@'%O if M=='OnPersist' else '??_G%s@NScript@@'%O, '??_E%s@NScript@@'%O,'??1%s@NScript@@'%O))]
    print(a,cat[a][2],O,M[:9],'ctor=%s'%ctor,h,syms)
