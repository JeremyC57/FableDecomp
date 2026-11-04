import sys,re
sys.path.insert(0,'/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/wave35d/scripts')
from common import *
ent=ps1_entries(); cat=catalog(); E,F=pdb()
D='rebuild/candidates/manual/naming/2026-11-04_wave35a/DEFERRED.tsv'
for l in open(ROOT+'/'+D):
    q=l.rstrip('\n').split('\t')
    if len(q)<3 or not re.search(r'is not the PDB|leaf not',q[2]): continue
    m=re.search(r'(?:row 0x|Alloc )([0-9a-f]{8})',q[2]); s=m.group(1)
    e=ent.get(s); src=e['Source'] if e else None
    lf=None
    if src:
        t=open(ROOT+'/rebuild/src/compiled/'+src,errors='replace').read()
        mm=re.search(r'^//\s*Leaf:\s*(\S+)',t,re.M); lf=mm.group(1) if mm else None
    sa=e['Source'] if (e:=ent.get(q[0])) else None
    print(q[0],s,cat[s][1],lf,'E' if lf in E else '-','F' if lf in F else '-',src,'|',sa,sep='\t')
