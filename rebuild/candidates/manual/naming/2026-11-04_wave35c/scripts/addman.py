# addman.py <fam> <addr> <module> <sym> <draftfile> [method]: register a hand-written respelled source in <fam>/gen.tsv
import sys, os, json, re
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from c35 import *
ent=ps1_entries(); cat=catalog()
fam,a,mod,sym,draft=sys.argv[1:6]
name=cat[a][1]
meth=sys.argv[6] if len(sys.argv)>6 else re.sub(r'\(.*\)$','',name).rsplit('::',1)[-1]
s=lambda x: re.sub(r'[^A-Za-z0-9]','',x)
base=(s(mod or '_global')[:80]+'_'+s(meth)[:48]+'_'+a)[:180]
nsrc=os.path.dirname(ent[a]['Source'])+'/'+base+'.cpp'
ntest=os.path.dirname(ent[a]['TestSource'])+'/'+base+'_test.cpp'
txt=open(draft,encoding='utf-8').read()
m=re.search(r'^//\s*Leaf:\s*(\S+)',txt,re.M)
assert m and m.group(1)==sym, ('leaf line', m and m.group(1), sym)
write(os.path.join(fam,'src',nsrc),txt)
g=os.path.join(fam,'gen.tsv')
lines=[l for l in (open(g).read().splitlines() if os.path.exists(g) else []) if not l.startswith(a+'\t')]
lines.append('\t'.join([a,'OK',json.dumps(dict(name=name,module=mod,sym=sym,old_src=ent[a]['Source'],new_src=nsrc,old_test=ent[a]['TestSource'],new_test=ntest,old_module=ent[a].get('Module',''),old_name=name))]))
open(g,'w').write('\n'.join(lines)+'\n')
print(a,nsrc)
