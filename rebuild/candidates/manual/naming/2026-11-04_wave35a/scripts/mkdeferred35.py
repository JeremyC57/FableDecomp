# mkdeferred35.py: every ROWS.tsv row without a final PASS, with its reason
import sys,os,json,re,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from lib35 import *
cat=catalog()
rows=[l.rstrip('\n').split('\t') for l in open(W+'/ROWS.tsv')][1:]
passed={}
for l in open(W+'/final/pass.tsv'):
    q=l.rstrip('\n').split('\t'); passed[q[2]]=(q[0],q[3])
why={}
for f in ['sdd','def','sf1','gesn','ent','op','op2','opfmd','ent2']:
    gp=W+'/'+f+'/gen.tsv'
    if not os.path.exists(gp): continue
    for l in open(gp):
        a=l.rstrip('\n').split('\t')
        if a[1]=='DEFER': why[a[0]]='%s: %s'%(f,a[2])
out=['address\tcatalog_name\treason']; c=collections.Counter()
for a,cn,leaf,src in rows:
    if a in passed and passed[a][0]=='PASS': continue
    if a in passed: r='final recheck: '+passed[a][1]; k='final_fail'
    elif cn.startswith('CActiveEntityScript<'):
        r=('HOLD (DECISIONS_W29_RESOLVED D1, unchanged): retail expands the implicit ~CActiveEntityScriptBase inside every CActiveEntityScript<T> ??_G '
           'while calling ~CSpawnedFuncBase / ~CCountedPointer / ~CScriptThing out of line; VC7.1 RTM over engine/CActiveEntityScript.h emits one call to the '
           'implicit base dtor instead. The wave 28 stand-in still grades byte-identical; the catalog name stands.'); k='CAES_D1'
    elif cn.endswith('::OnPredicateFail') and int(cat[a][2])>1 and a not in why:
        r=('not attempted: nontrivial OnPredicateFail body (%s bytes); its parent / master-data / Me members need a class model that names them '
           '(the landed Alloc model has no outer members and the shared header, where one exists, lacks the nested class or its OnPredicateFail); '
           'respell with the class model (header completion) wave'%cat[a][2]); k='opf_nontrivial'
    elif a in why: r=why[a]; k=why[a].split(':')[0]+':'+re.sub(r'0x[0-9a-fA-F]+|[0-9a-f]{8}|\(.*','',why[a].split(':',1)[1])[:50].strip()
    else: r='no generator covers this shape'; k='none'
    c[k]+=1; out.append('%s\t%s\t%s'%(a,cn,r))
open(W+'/DEFERRED.tsv','w').write('\n'.join(out)+'\n')
print(len(out)-1); [print(v,k) for k,v in c.most_common()]
