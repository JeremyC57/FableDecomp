import os, itertools
W=os.environ['W']
s=open(W+'/tmp/c1.cpp').read()
F="{o}X = {f}Y * {r}Z - {f}Z * {r}Y; {o}Y = {r}X * {f}Z - {f}X * {r}Z; {o}Z = {f}X * {r}Y - {r}X * {f}Y;"
arg={'o':'m_top','f':'m_front','r':'m_right'}
pn={'o':'out','f':'u','r':'v'}
out=[]
# free/static helper, param permutations, refs and pointers
for perm in itertools.permutations('ofr'):
    for kind in ('ref','ptr'):
        sig=', '.join(('%sC3DVector%s %s'%('' if c=='o' else 'const ', '&' if kind=='ref' else '*', pn[c])) for c in perm)
        acc={c:(pn[c]+('.' if kind=='ref' else '->')) for c in 'ofr'}
        body=F.format(o=acc['o'],f=acc['f'],r=acc['r'])
        helper='inline void CrossHelper(%s) { %s }\n'%(sig,body)
        call='CrossHelper(%s);'%', '.join((('' if kind=='ref' else '&')+arg[c]) for c in perm)
        out.append(('free_%s_%s'%(''.join(perm),kind), helper, call))
# member forms: this = each role
for th in 'ofr':
    rest=[c for c in 'ofr' if c!=th]
    for perm in itertools.permutations(rest):
        sig=', '.join(('%sC3DVector& %s'%('' if c=='o' else 'const ', pn[c])) for c in perm)
        acc={c:(pn[c]+'.') for c in 'ofr'}; acc[th]=''
        body=F.format(o=acc['o'],f=acc['f'],r=acc['r'])
        const=' const' if th!='o' else ''
        helper='M%s(%s)%s { %s }'%(th+''.join(perm),sig,const,body)
        call='%s.M%s(%s);'%(arg[th],th+''.join(perm),', '.join(arg[c] for c in perm))
        out.append(('mem_%s_%s'%(th,''.join(perm)), helper, call))
for name,helper,call in out:
    t=s.replace('m_top.SetCross(m_front, m_right);',call)
    if name.startswith('mem'):
        t=t.replace('    void SetCross(','    void '+helper+'\n    void SetCross(')
    else:
        t=t.replace('extern unsigned',helper+'extern unsigned')
    open('v_%s.cpp'%name,'w').write(t)
