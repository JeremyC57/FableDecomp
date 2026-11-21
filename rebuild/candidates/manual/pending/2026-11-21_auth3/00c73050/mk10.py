import re
src=open('b0.cpp').read()
pat=re.compile(r'if \(!(pfn\w+)\)\n(\s+)\1 = \((\w+)\)\(UseALGetProcAddress \? GetProcAddress\("(\w+)"\)\n\s+: ::GetProcAddress\(Module, "\4"\)\);\n(\s+)\1\(')
def mk(name, which):
    def rep(m):
        p,ind,t,n,ind2=m.groups()
        if n not in which: return m.group(0)
        return (f'{t} fn = {p};\n{ind2}if (!fn) {{\n{ind}fn = ({t})(UseALGetProcAddress ? GetProcAddress("{n}") : ::GetProcAddress(Module, "{n}"));\n'
                f'{ind}{p} = fn;\n{ind2}}}\n{ind2}fn(')
    s,k=pat.subn(rep,src); assert s!=src, name; open(name+'.cpp','w').write(s)
mk('v10',{'alSourcePlay','alGetSourcei','alSourceUnqueueBuffers'})
mk('v11',{'alSourceUnqueueBuffers'})
