# include-order variants: each include moved to the front, each moved to the back, full reverse
import re
def _split(s):
    L=s.split('\n'); idx=[i for i,l in enumerate(L) if l.startswith('#include')]
    return L, idx
def mk(order_fn):
    def f(s):
        L,idx=_split(s); inc=[L[i] for i in idx]; new=order_fn(inc)
        for i,l in zip(idx,new): L[i]=l
        return '\n'.join(L)
    return f
VARIANTS=[]
import os
n=int(os.environ.get('NINC','0'))
for k in range(1,n): VARIANTS.append(('front%d'%k, mk(lambda inc,k=k: [inc[k]]+inc[:k]+inc[k+1:])))
for k in range(0,n-1): VARIANTS.append(('back%d'%k, mk(lambda inc,k=k: inc[:k]+inc[k+1:]+[inc[k]])))
VARIANTS.append(('reverse', mk(lambda inc: inc[::-1])))
