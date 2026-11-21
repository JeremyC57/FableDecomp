import re
src=open('b0.cpp').read()
pat=re.compile(r'if \(!(pfn\w+)\)\n(\s+)\1 = \((\w+)\)\(UseALGetProcAddress \? GetProcAddress\("(\w+)"\)\n\s+: ::GetProcAddress\(Module, "\4"\)\);\n(\s+)\1\(')
def mk(name, which, form):
    def rep(m):
        p,ind,t,n,ind2=m.groups()
        if n not in which: return m.group(0)
        if form=='proc':
            return (f'if (!{p}) {{\n{ind}void* proc = UseALGetProcAddress ? GetProcAddress("{n}") : ::GetProcAddress(Module, "{n}");\n'
                    f'{ind}{p} = ({t})proc;\n{ind2}}}\n{ind2}{p}(')
    s,k=pat.subn(rep,src); assert s!=src, name; open(name+'.cpp','w').write(s)
mk('v12',{'alSourcePlay','alGetSourcei','alSourceUnqueueBuffers'},'proc')
