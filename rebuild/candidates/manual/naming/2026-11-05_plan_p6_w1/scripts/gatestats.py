# gatestats.py W : tabulate the gate's failure classes (first error code + text) and hdrmig's deferral reasons of wave W.
import sys,os,re,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__))); import wavelib as L
W=sys.argv[1]; c=collections.Counter(); ex={}
for r in L.tsv(W+'/gate.tsv'):
    if r['verdict']=='PASS': c['PASS']+=1; continue
    why=r['reason']; m=re.search(r'error (C\d+): (.{0,60})',why)
    k=(m.group(1)+' '+m.group(2)) if m else why[:50]; c[k]+=1; ex.setdefault(k,r['address'])
for k,n in c.most_common(30): print('%4d %s  e.g. %s'%(n,k,ex.get(k,'')))
if os.path.exists(W+'/hdrmig.tsv'):
    lk=collections.Counter(); kinds=collections.Counter()
    for r in L.tsv(W+'/hdrmig.tsv'):
        if r['status']=='rewritten': continue
        kinds[r['note'].split(' ')[0]]+=1
        if r['note'].startswith(('header_lacks','unresolved_call')):
            for n in r['note'].split(' ',1)[1].split(','): lk[n]+=1
    print('---- deferral kinds:',kinds.most_common()); print('---- header_lacks / unresolved names')
    for k,n in lk.most_common(40): print('%4d %s'%(n,k))
