# every multi-line ternary resolver `if (!pfnX)\n  pfnX = (T)(UseALGetProcAddress ? GetProcAddress("n")\n  : ::GetProcAddress(Module, "n"));` -> proc form
import re,sys
pat=re.compile(r'if \(!(pfn\w+)\)\n(\s+)\1 = \((\w+)\)\(UseALGetProcAddress \? GetProcAddress\("(\w+)"\)\n\s+: ::GetProcAddress\(Module, "\4"\)\);\n')
def rep(m):
    p,ind,t,n=m.groups(); i0=ind[:-4] if len(ind)>=4 else ''
    return (f'if (!{p}) {{\n{ind}void* proc = UseALGetProcAddress ? GetProcAddress("{n}") : ::GetProcAddress(Module, "{n}");\n'
            f'{ind}{p} = ({t})proc;\n{i0}}}\n')
s=open(sys.argv[1]).read(); s2,k=pat.subn(rep,s)
left=len(re.findall(r'if *\( *! *pfn\w+ *\)',s2))-k
open(sys.argv[2],'w').write(s2); print(k, left)
