# map entity class (Inner@Outer) -> landed Alloc row (addr, source, leaf)
import re,sys
sys.path.insert(0,'/home/user/FableDecomp/rebuild/candidates/manual/naming/2026-11-03_wave33/scripts')
from common import ps1_entries, catalog, ROOT
def alloc_map():
    ent=ps1_entries(); cat=catalog(); m={}
    for a,q in cat.items():
        if not q[1].endswith('::Alloc') or a not in ent: continue
        try: s=open(ROOT+'/rebuild/src/compiled/'+ent[a]['Source'],encoding='utf-8').read()
        except: continue
        lm=re.search(r'^//\s*Leaf:\s*(\?Alloc@(\S+?)@NScript@@SI\S*)',s,re.M)
        if not lm: continue
        m.setdefault(lm.group(2),[]).append((a,ent[a]['Source'],lm.group(1),q[1]))
    return m
