exec(open('exp361.py').read().split("out,P,h=sim.schedule(R1); print('base'")[0])
fp=[i for i in range(60,76)]
def F(N,k):
    fps=[i for i in N if N[i]['unit']==2 and i<82]
    nodes=[]
    for j in range(k):
        nodes.append((90+j,'fld',2,4,[(81,1)]+[(i,0) for i in fps if i!=75]+[(75,2)]+[(90+q,0) for q in range(j)]))
    return append(N,nodes)
def seq(N):
    out,P,h=sim.schedule(N); return [i for c,s,i in out if N[i]['op']!='0x269']
base=seq(R1); good=seq(F(remove(R1,62),1))
gs=[i for i in good if i<90]
print('base vs rm62+1 (ignoring PAREN/fld90):', 'same' if base==gs else 'differ')
for k,(a,b) in enumerate(zip(base,gs)):
    if a!=b: print(' first diff pos',k, base[k:k+8], gs[k:k+8]); break
def insert(N,a,new=95):
    N=copy.deepcopy(N); succ=N[a]['succ']
    fpcons=[(s,l) for s,l in succ if l>0 and N.get(s,{}).get('unit')==2]
    if not fpcons: return None
    c,L=max(fpcons,key=lambda x:x[1])
    N[a]['succ']=[(s,(0 if s==c else l)) for s,l in succ]+[(new,L)]
    N[new]=dict(idx=new,op='0x269',fl=0,unit=0,succ=[(s,0) for s,l in succ])
    return N
# renumber issue: new idx 95 > others; ties use idx asc. place idx as a+0.5
def insert2(N,a):
    M=insert(N,a,new=a+0.5); return M
two=remove(remove(R1,62),70)
for a in sorted(i for i in R1 if R1[i]['unit']==2 and i<60):
    M=insert2(two,a)
    if M is None: continue
    s=[i for i in seq(F(M,1)) if i<90 and i==int(i)]
    print('rm62,70 +PAREN after',a,R1[a]['op'],'->','RETAIL-LIKE' if s==gs else 'no')
