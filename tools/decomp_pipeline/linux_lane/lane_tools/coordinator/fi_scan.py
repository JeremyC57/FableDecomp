import re,glob,collections,sys
def strip(t):
    out=[];i=0;n=len(t)
    while i<n:
        c=t[i]
        if t.startswith('//',i):
            j=t.find('\n',i); j=n if j<0 else j; out.append(' '*(j-i)); i=j
        elif t.startswith('/*',i):
            j=t.find('*/',i+2); j=n if j<0 else j+2; out.append(re.sub(r'[^\n]',' ',t[i:j])); i=j
        elif c in '"\'':
            j=i+1
            while j<n and t[j]!=c:
                if t[j]=='\\': j+=1
                j+=1
            out.append(c+' '*(j-i-1)+(c if j<n else '')); i=j+1
        else: out.append(c); i+=1
    return ''.join(out)
def analyse(f):
    t=strip(open(f,encoding='latin-1').read()); L=t.split('\n')
    stack=[];res=[]
    for i,l in enumerate(L):
        s=l.strip()
        m=re.match(r'#\s*(if|ifdef|ifndef|elif|else|endif)\b(.*)',s)
        if m:
            k,arg=m.groups()
            if k in('if','ifdef','ifndef'): stack.append(('_MSC_VER' in arg and k!='ifndef' and '!' not in arg))
            elif k in ('else','elif') and stack: stack[-1]=False
            elif k=='endif' and stack: stack.pop()
            continue
        if re.search(r'\b__forceinline\b',l):
            kind='define' if s.startswith('#') else ('guard' if any(stack) else 'bare')
            res.append((i+1,kind))
    shim=bool(re.search(r'#\s*define\s+__forceinline\b',t))
    return res,shim
if __name__=='__main__':
    for root in sys.argv[1:]:
        tot=collections.Counter(); files=collections.Counter()
        for f in glob.glob(root+'/**/*.*',recursive=True):
            if not f.endswith(('.h','.hpp','.cpp','.inl')): continue
            r,shim=analyse(f)
            for _,k in r:
                kk=k+('_shim' if shim and k=='bare' else ''); tot[kk]+=1
            ks=set(k+('_shim' if shim and k=='bare' else '') for _,k in r)
            for k in ks: files[k]+=1
        print(root, dict(tot), dict(files))
