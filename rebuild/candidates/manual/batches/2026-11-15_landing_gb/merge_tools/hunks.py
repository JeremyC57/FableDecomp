import sys,re,json,os
# usage: hunks.py dump <conf> | hunks.py apply <conf> <res.json> <out>
def parse(path):
    L=open(path).read().split('\n'); segs=[]; i=0; cur=[]
    while i<len(L):
        if L[i].startswith('<<<<<<< '):
            segs.append(('t',cur)); cur=[]
            o=[];b=[];t=[]; i+=1
            while not L[i].startswith('||||||| '): o.append(L[i]); i+=1
            i+=1
            while not L[i].startswith('======='): b.append(L[i]); i+=1
            i+=1
            while not L[i].startswith('>>>>>>> '): t.append(L[i]); i+=1
            segs.append(('h',(o,b,t)))
        else: cur.append(L[i])
        i+=1
    segs.append(('t',cur)); return segs
ws=lambda x:[re.sub(r'\s+','',l) for l in x if l.strip()]
def auto(o,b,t):
    if ws(o)==ws(t): return o
    wo,wt=ws(o),ws(t)
    if wo and wt[-len(wo):]==wo:  # theirs = extra + ours
        # keep theirs prefix lines up to the ours-equivalent tail
        n=0;k=len(t)
        while n<len(wo):
            k-=1
            if t[k].strip(): n+=1
        return t[:k]+o
    return None
mode=sys.argv[1]; segs=parse(sys.argv[2])
if mode=='dump':
    n=0
    for k,v in segs:
        if k=='h':
            a=auto(*v)
            print('=== HUNK',n,'AUTO' if a is not None else 'MANUAL')
            if a is None:
                for nm,x in zip(('OURS','BASE','THEIRS'),v):
                    print('---',nm); print('\n'.join(x))
            n+=1
else:
    res=json.load(open(sys.argv[3])) if os.path.exists(sys.argv[3]) else {}
    out=[];n=0
    for k,v in segs:
        if k=='t': out+=v; continue
        r=res.get(str(n)); o,b,t=v
        if r is None: a=auto(o,b,t); assert a is not None,(sys.argv[2],n); out+=a
        elif r=='ours': out+=o
        elif r=='theirs': out+=t
        elif r=='ot': out+=o+t
        elif r=='to': out+=t+o
        else: out+=r.split('\n') if isinstance(r,str) else r
        n+=1
    open(sys.argv[4],'w').write('\n'.join(out))
