import json,sys,copy
sys.path.insert(0,'.')
import sim
W='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c'
t=json.load(open(W+'/tmp/dag_361d0.json'))
dags=[e[1] for e in t['events'] if e[0]=='dag']
R1=sim.build(dags[0]); R2=sim.build(dags[1])
def tail(N,out,k=26):
    return ' '.join('%s#%s'%(N[i]['op'],i) for c,s,i in out[-k:])
def remove(N,x):
    N=copy.deepcopy(N); n=N.pop(x)
    for i in N:
        new=[]
        for s,l in N[i]['succ']:
            if s==x:
                for s2,l2 in n['succ']: new.append((s2,l+l2))
            else: new.append((s,l))
        d={}
        for s,l in new: d[s]=max(d.get(s,-1),l)
        N[i]['succ']=sorted(d.items(),reverse=True)
    return N
def append(N,nodes,end=82):
    # nodes: list of (newidx, op, unit, fl, preds[(idx,lat)])
    N=copy.deepcopy(N); endn=N.pop(end)
    for idx,op,unit,fl,preds in nodes:
        N[idx]=dict(idx=idx,op=op,fl=fl,unit=unit,succ=[(end+50,0)])
        for p,l in preds:
            if p in N: N[p]["succ"].append((idx,l))
    endn['idx']=end+50; N[end+50]=endn
    # repoint old end edges
    for i in N:
        N[i]['succ']=[((end+50) if s==end else s,l) for s,l in N[i]['succ']]
    return N
out,P,h=sim.schedule(R1); print('base     ',tail(R1,out))
F1=(90,'fld',2,4,[(81,1)]); F2=(91,'fld',2,4,[(81,1)])
for x in (None,62,64,70,72):
    N=R1 if x is None else remove(R1,x)
    for extra in ([],[F1],[F1,F2]):
        M=append(N,extra) if extra else N
        out,P,h=sim.schedule(M); print('rm',x,'+%d'%len(extra),tail(M,out))
print('--- fld with x87 stack-order preds')
fp=[i for i in range(60,76)]
F1=(90,'fld',2,4,[(81,1)]+[(i,0) for i in fp[:-1]]+[(75,2)])
F2=(91,'fld',2,4,[(81,1)]+[(i,0) for i in fp[:-1]]+[(75,2),(90,0)])
for x in (None,62,64,70,72):
    N=R1 if x is None else remove(R1,x)
    for extra in ([F1],[F1,F2]):
        M=append(N,extra)
        out,P,h=sim.schedule(M); print('rm',x,'+%d'%len(extra),tail(M,out,18))
print('--- rm 62+70')
N=remove(remove(R1,62),70)
for extra in ([F1],[F1,F2]):
    M=append(N,extra); out,P,h=sim.schedule(M); print('rm62,70 +%d'%len(extra),tail(M,out,18))
