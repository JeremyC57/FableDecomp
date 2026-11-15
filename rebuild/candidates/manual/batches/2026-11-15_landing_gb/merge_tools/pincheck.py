import re,sys,collections
old,new=sys.argv[1],sys.argv[2]
R=re.compile(r'FABLE_STATIC_ASSERT(?:_AT\(\s*(\d+)\s*,|\()(.*)\);')
def eff(path):
    m=collections.defaultdict(list)
    for i,l in enumerate(open(path,errors='replace')):
        x=R.search(l)
        if x: m[re.sub(r'\s+','',x.group(2))].append(int(x.group(1)) if x.group(1) else i+1)
    return m
bad=0
for h in open(sys.argv[3]).read().split():
    a=eff(old+'/'+h); b=eff(new+'/'+h)
    for k,v in a.items():
        w=list(b.get(k,[]))
        miss=[n for n in v if not (n in w and not w.remove(n))]
        if miss: bad+=1; print(h,'old',v,'new',b.get(k),k[:70])
print('mismatching',bad)
