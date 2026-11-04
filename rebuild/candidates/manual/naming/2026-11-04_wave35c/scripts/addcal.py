# addcal.py addr... : register callers/<addr>.cpp into cal family (no rename)
import sys,json,os
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from c35 import *
ent=ps1_entries(); cat=catalog()
lines=open('cal/gen.tsv').read().splitlines()
for a in sys.argv[1:]:
    e=ent[a]; write(os.path.join('cal','src',e['Source']),open('callers/'+a+'.cpp').read())
    lines=[l for l in lines if not l.startswith(a)]
    lines.append('\t'.join([a,'OK',json.dumps(dict(name=cat[a][1],module=e.get('Module',''),sym='',old_src=e['Source'],new_src=e['Source'],old_test=e['TestSource'],new_test=e['TestSource'],old_module=e.get('Module',''),old_name=cat[a][1]))]))
    print(a,e['Source'])
open('cal/gen.tsv','w').write('\n'.join(lines)+'\n')
