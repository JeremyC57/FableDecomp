import os, itertools
W=os.environ['W']
s=open(W+'/tmp/c1.cpp').read()
P=[('{f}Y','{r}Z'),('{f}Z','{r}Y'),('{r}X','{f}Z'),('{f}X','{r}Z'),('{f}X','{r}Y'),('{r}X','{f}Y')]
arg={'o':'m_top','f':'m_front','r':'m_right'}; pn={'o':'out','f':'u','r':'v'}
for perm in itertools.permutations('ofr'):
  for bits in itertools.product([0,1],repeat=6):
    q=[(p if not b else (p[1],p[0])) for p,b in zip(P,bits)]
    m=['%s * %s'%x for x in q]
    body='{o}X = %s - %s; {o}Y = %s - %s; {o}Z = %s - %s;'%tuple(m)
    sig=', '.join(('%sC3DVector& %s'%('' if c=='o' else 'const ', pn[c])) for c in perm)
    acc={c:pn[c]+'.' for c in 'ofr'}
    helper='inline void CrossHelper(%s) { %s }\n'%(sig,body.format(**acc))
    call='CrossHelper(%s);'%', '.join(arg[c] for c in perm)
    t=s.replace('m_top.SetCross(m_front, m_right);',call).replace('extern unsigned',helper+'extern unsigned')
    open('v_%s_%s.cpp'%(''.join(perm),''.join(map(str,bits))),'w').write(t)
