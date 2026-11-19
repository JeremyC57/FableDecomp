import os, itertools
W=os.environ['W']
s=open(W+'/tmp/c1.cpp').read()
arg={'o':'m_top','f':'m_front','r':'m_right','d':'0'}; pn={'o':'out','f':'u','r':'v'}
F="{o}X = {f}Y * {r}Z - {f}Z * {r}Y; {o}Y = {r}X * {f}Z - {f}X * {r}Z; {o}Z = {f}X * {r}Y - {r}X * {f}Y;"
for perm in itertools.permutations('ofr'):
  for k in range(4):
   for pos in range(4):
    L=list(perm)
    for i in range(k): L.insert(pos,'d')
    sig=', '.join(('int d%d'%i if c=='d' else '%sC3DVector& %s'%('' if c=='o' else 'const ', pn[c])) for i,c in enumerate(L))
    acc={c:pn[c]+'.' for c in 'ofr'}
    helper='inline void CrossHelper(%s) { %s }\n'%(sig,F.format(**acc))
    call='CrossHelper(%s);'%', '.join(arg[c] for c in L)
    t=s.replace('m_top.SetCross(m_front, m_right);',call).replace('extern unsigned',helper+'extern unsigned')
    open('v_%s_k%dp%d.cpp'%(''.join(perm),k,pos),'w').write(t)
