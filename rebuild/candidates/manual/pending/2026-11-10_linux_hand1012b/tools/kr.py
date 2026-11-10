# K&R quicksort as c2.dll 0x107AE0C4 (comparator 0x107AE9C2: density w*1000/size descending, ties 0 = no swap)
def dens(s): return s[1]*1000//s[2]
def qs(v,l,r):
    while l<r:
        m=(l+r)//2; v[l],v[m]=v[m],v[l]; last=l
        for i in range(l+1,r+1):
            if dens(v[l])<dens(v[i]): last+=1; v[last],v[i]=v[i],v[last]
        v[l],v[last]=v[last],v[l]
        qs(v,l,last-1); l=last+1
def run(v): v=list(v); qs(v,0,len(v)-1); return [x[0] for x in v]
if __name__=='__main__':
    o=[('path',11,4),('fp+hs',11,4),('name',5,4),('qs',5,4),('pat',4,4),('tmp',4,4),('info',1,8),('fn',2,524)]
    print(run(o))
    o2=[('path',11,4),('fp+hs',11,4),('name',5,4),('qs',5,4),('pat',4,4),('t1',2,4),('t2',2,4),('info',1,8),('fn',2,524)]
    print(run(o2))
