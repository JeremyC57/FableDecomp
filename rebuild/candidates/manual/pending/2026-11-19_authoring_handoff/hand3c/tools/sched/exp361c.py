exec(open('exp361b.py').read().split("two=remove(")[0])
target=[i for i in seq(remove(R1,62))]
def ins_any(N,a,new):
    N=copy.deepcopy(N); succ=N[a]['succ']
    fpc=[(s,l) for s,l in succ if l>0]
    if not fpc: return None
    c,L=max(fpc,key=lambda x:x[1])
    N[a]['succ']=[(s,(0 if s==c else l)) for s,l in succ]+[(new,L)]
    N[new]=dict(idx=new,op='0x269',fl=0,unit=0,succ=[(s,0) for s,l in succ])
    return N
print('rm62 alone (80 nodes) == retail-like reference')
ok=[]
for base_rm in ([62],[64],[70],[72],[]):
    B=R1
    for x in base_rm: B=remove(B,x)
    for a in sorted(i for i in R1 if i not in (0,82) and i not in base_rm):
        M=ins_any(B,a,a+0.5)
        if M is None: continue
        s=[i for i in seq(M) if i==int(i)]
        if s==target: ok.append((base_rm,a,R1[a]['op']))
print(ok)
