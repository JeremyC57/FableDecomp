import sys,re
for ln in open(sys.argv[1]):
    a,f=ln.split()
    src=open(f).read().split('\n')
    depths=[];regions=[];cur=None
    for i,l in enumerate(src):
        s=l.strip()
        if s.startswith('#pragma inline_depth'):
            m=re.search(r'inline_depth\(\s*(\d*)\s*\)',s); d=m.group(1)
            depths.append(d or '-')
            if cur is not None: regions.append(cur); cur=None
            if d!='' : cur=[d,[]]
        elif cur is not None and s: cur[1].append(s)
    kinds=[]
    for d,body in regions:
        txt=' '.join(body)
        k='ret' if 'return' in txt else ('class' if re.search(r'\b(struct|class)\b.*\{',txt) else 'stmt')
        kinds.append(f'{d}:{k}:{len(body)}')
    fi='FI' if '__forceinline' in '\n'.join(src) else ''
    print(a,fi,' '.join(kinds),sep='\t')
