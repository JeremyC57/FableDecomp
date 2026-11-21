import re,sys
src=open('b0.cpp').read()
def ifelse(s, which):
    # rewrite "if (!pfnX)\n  pfnX = (T)(Use ? GetProcAddress("n") : ::GetProcAddress(Module, "n"));" into if/else form
    pat=re.compile(r'if \(!(pfn\w+)\)\n(\s+)\1 = \((\w+)\)\(UseALGetProcAddress \? GetProcAddress\("(\w+)"\)\n\s+: ::GetProcAddress\(Module, "\4"\)\);')
    def rep(m):
        p,ind,t,n=m.groups()
        if n not in which: return m.group(0)
        i0=ind[:-4]
        return (f'if (!{p}) {{\n{ind}if (UseALGetProcAddress)\n{ind}    {p} = ({t})GetProcAddress("{n}");\n'
                f'{ind}else\n{ind}    {p} = ({t})::GetProcAddress(Module, "{n}");\n{i0}}}')
    s2,k=pat.subn(rep,s); return s2
for name,which in [('v7',{'alSourcePlay','alGetSourcei','alSourceUnqueueBuffers'}),('v8',{'alSourcePlay'}),('v9',{'alSourceUnqueueBuffers'})]:
    s=ifelse(src,which); assert s!=src; open(name+'.cpp','w').write(s)
