# spell.py: per in-scope target, the spellings actually emitted today (definer + callers) -> logs/SPELL.tsv
import sys,os
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from vcommon import *
def load(scanf=W+'/logs/scan_base.tsv',deff=W+'/logs/defs_base.tsv'):
    defs={}
    for l in open(deff):
        a,st,s=l.rstrip('\n').split('\t'); defs[a]=(st,s)
    use=collections.defaultdict(lambda: collections.defaultdict(set))
    for l in open(scanf):
        q=l.rstrip('\n').split('\t')
        if q[1]!='OK' or not q[2]: continue
        c,_,s,t,k=q
        if scope(t)!='out': use[t][s].add(c)
    return defs,use
if __name__=='__main__':
    defs,use=load(); inv=inventory(); cat=catalog()
    tg=set(use)|set(a for a in defs if scope(a)!='out')
    n=collections.Counter()
    with open(W+'/logs/SPELL.tsv','w') as o:
        o.write('target\tcatalog\tdef_status\tdef_symbol\tn_spellings\tspellings(callers)\n')
        for t in sorted(tg):
            ds=defs.get(t,('NOTLANDED',''))
            sp=dict((s,sorted(c)) for s,c in use.get(t,{}).items())
            allsp=set(sp)|({ds[1]} if ds[1] and ds[0] in('OK','MERGED') else set())
            n[(scope(t),len(allsp)>1)]+=1
            o.write('\t'.join([t,cat.get(t,['','?'])[1],ds[0],ds[1],str(len(allsp)),' | '.join('%s[%s]'%(s,','.join(c)) for s,c in sorted(sp.items(),key=lambda x:-len(x[1])))])+'\n')
    print(n)
