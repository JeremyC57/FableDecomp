import os, itertools
W=os.environ['W']
s=open(W+'/tmp/c1.cpp').read()
arg={'o':'m_top','f':'m_front','r':'m_right','d':'0'}; pn={'o':'out','f':'u','r':'v'}
F="{o}X = {f}Y * {r}Z - {f}Z * {r}Y; {o}Y = {r}X * {f}Z - {f}X * {r}Z; {o}Z = {f}X * {r}Y - {r}X * {f}Y;"
def comps(k,n):
    if n==1: yield (k,); return
    for i in range(k+1):
        for r in comps(k-i,n-1): yield (i,)+r
for perm in itertools.permutations('ofr'):
  for k in range(7):
   for c in comps(k,4):
    L=['d']*c[0]+[perm[0]]+['d']*c[1]+[perm[1]]+['d']*c[2]+[perm[2]]+['d']*c[3]
    sig=', '.join(('int d%d'%i if x=='d' else '%sC3DVector& %s'%('' if x=='o' else 'const ', pn[x])) for i,x in enumerate(L))
    acc={x:pn[x]+'.' for x in 'ofr'}
    helper='inline void CrossHelper(%s) { %s }\n'%(sig,F.format(**acc))
    call='CrossHelper(%s);'%', '.join(arg[x] for x in L)
    t=s.replace('m_top.SetCross(m_front, m_right);',call).replace('extern unsigned',helper+'extern unsigned')
    open('v_%s_%s.cpp'%(''.join(perm),''.join(map(str,c))),'w').write(t)
