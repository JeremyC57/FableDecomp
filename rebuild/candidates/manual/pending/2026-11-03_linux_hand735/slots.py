import sys,re,collections
L=open(sys.argv[1]).read().split('\n')
i=[k for k,l in enumerate(L) if l.startswith('--- retail')][0]; j=[k for k,l in enumerate(L) if l.startswith('--- built')][0]
A=[l for l in L[i+1:j] if l.strip()]; B=[l for l in L[j+1:] if l.strip()]
m=collections.defaultdict(collections.Counter); first={}
for k,(a,b) in enumerate(zip(A,B)):
    ra=re.findall(r'\[ebp ([+-]) (0x[0-9a-f]+|\d+)\]',a); rb=re.findall(r'\[ebp ([+-]) (0x[0-9a-f]+|\d+)\]',b)
    for x,y in zip(ra,rb):
        va=int(x[1],0)*(1 if x[0]=='+' else -1); vb=int(y[1],0)*(1 if y[0]=='+' else -1)
        m[va][vb]+=1; first.setdefault(va,a[:8])
for va in sorted(m):
    s=' '.join('%x:%d'%(v,c) for v,c in m[va].items())
    print('%5x %s %s%s'%(va,first[va],s,'' if list(m[va])==[va] else '  <--'))
