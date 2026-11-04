import os,re,sys
ROOT='/home/user/FableDecomp'
W=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
def rows():
    R=[]
    for i,l in enumerate(open(W+'/ROWS.tsv')):
        if i==0: continue
        a=l.rstrip('\n').split('\t')
        R.append(dict(addr=a[0],name=a[1],cur=a[2],src=a[3]))
    return R
def src(r): return open(ROOT+'/rebuild/src/compiled/'+r['src'],encoding='utf-8').read()
