import os, itertools
W=os.environ['W']
s=open(W+'/tmp/c1.cpp').read()
st={'X':"{o}X = {f}Y * {r}Z - {f}Z * {r}Y;",'Y':"{o}Y = {r}X * {f}Z - {f}X * {r}Z;",'Z':"{o}Z = {f}X * {r}Y - {r}X * {f}Y;"}
arg={'o':'m_top','f':'m_front','r':'m_right'}; pn={'o':'out','f':'u','r':'v'}
for perm in itertools.permutations('ofr'):
  for so in itertools.permutations('XYZ'):
    sig=', '.join(('%sC3DVector& %s'%('' if c=='o' else 'const ', pn[c])) for c in perm)
    acc={c:pn[c]+'.' for c in 'ofr'}
    body=' '.join(st[k].format(**acc) for k in so)
    helper='inline void CrossHelper(%s) { %s }\n'%(sig,body)
    call='CrossHelper(%s);'%', '.join(arg[c] for c in perm)
    t=s.replace('m_top.SetCross(m_front, m_right);',call).replace('extern unsigned',helper+'extern unsigned')
    open('v_%s_%s.cpp'%(''.join(perm),''.join(so)),'w').write(t)
