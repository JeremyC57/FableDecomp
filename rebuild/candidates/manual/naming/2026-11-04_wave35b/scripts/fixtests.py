# fixtests.py <famdir> <addr...>: for rows whose landed test links the source object under the old symbol, write an overlay test
#   that keeps the test's model and maps the old symbol onto the row's new one (/alternatename), then point gen.tsv at it
import sys,json,os
from common35 import *
fam=sys.argv[1]; rows=set(sys.argv[2:])
g=[l.rstrip('\n').split('\t') for l in open(fam+'/gen.tsv')]
n=0
for r in g:
    if r[0] not in rows or r[1]!='OK': continue
    d=json.loads(r[2])
    t=open(ROOT+'/rebuild/tests/'+d['old_test'],encoding='utf-8').read()
    old,new=d['old_sym'],d['leaf']
    if '/alternatename:' in t: continue
    pre='// Naming wave 35: the row now defines %s (it was landed as %s).\n// This behaviour model keeps the old spelling; the linker maps it onto the row\'s symbol (same thiscall/ecx ABI, same layout).\n#pragma comment(linker, "/alternatename:%s=%s")\n'%(new,old,old,new)
    tp=os.path.join(fam,'tests',d['new_test']); os.makedirs(os.path.dirname(tp),exist_ok=True)
    open(tp,'w').write(pre+t); d['test_path']=tp; d['test_fix']='alternatename'; r[2]=json.dumps(d); n+=1
open(fam+'/gen.tsv','w').write(''.join('\t'.join(x)+'\n' for x in g))
print('tests fixed',n)
