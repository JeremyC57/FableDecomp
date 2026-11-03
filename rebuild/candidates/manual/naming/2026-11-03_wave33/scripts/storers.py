import sys, collections, re
cat={}
for l in open('rebuild/oracles/auto-re-candidates.tsv'):
    a=l.rstrip('\n').split('\t')
    if len(a)>=3 and a[0]!='address': cat[a[0]]=a[1]
by=collections.defaultdict(set)
for l in open('rebuild/oracles/reloc_bindings.tsv'):
    a=l.rstrip('\n').split('\t')
    if len(a)<3 or a[0]=='symbol': continue
    by[a[1].lower()].add(a[2].lower())
out=open(sys.argv[2],'w')
for l in open(sys.argv[1]):
    a=l.rstrip('\n').split('\t')
    vt=a[2]
    calls=a[3]
    kind=re.sub(r'[0-9a-f]{8}=','',calls); kind=kind.replace(' ; MSVCR71.DLL::operator_delete','')
    st=sorted(by.get(vt,[]))
    out.write('%s\t%s\t%s\t%s\t%s\n'%(a[0],a[1],vt,kind,' ; '.join('%s=%s'%(s,cat.get(s,'?')) for s in st)))
