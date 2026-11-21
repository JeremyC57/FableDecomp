import re,sys
pat=re.compile(r'if \(!(pfn\w+)\)\n(\s+)\1 = \((\w+)\)\(UseALGetProcAddress \? GetProcAddress\("(\w+)"\)\n\s+: ::GetProcAddress\(Module, "\4"\)\);\n(\s+)\1\(')
def rep(m):
    p,ind,t,n,ind2=m.groups()
    return (f'if (!{p}) {{\n{ind}void* proc = UseALGetProcAddress ? GetProcAddress("{n}") : ::GetProcAddress(Module, "{n}");\n'
            f'{ind}{p} = ({t})proc;\n{ind2}}}\n{ind2}{p}(')
s=open(sys.argv[1]).read(); s2,k=pat.subn(rep,s); open(sys.argv[2],'w').write(s2); print(k)
