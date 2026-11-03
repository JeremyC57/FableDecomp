import sys,re,glob,os,collections
R='/home/user/FableDecomp/'
binds=collections.defaultdict(collections.Counter)
for l in open(R+'rebuild/oracles/reloc_bindings.tsv'):
    p=l.rstrip('\n').split('\t')
    if len(p)>=3: binds[p[1].lower()][p[0]]+=1
cat={}
for l in open(R+'rebuild/oracles/auto-re-candidates.tsv'):
    p=l.split('\t',3); cat[p[0].lower()]=p[1]
dis=sys.argv[1]
seen=[]
for l in open(dis):
    for m in re.finditer(r'0x([0-9a-f]{6,8})',l.split(None,2)[2] if len(l.split(None,2))>2 else ''):
        a=m.group(1).zfill(8)
        if int(a,16)>=0x400000 and a not in seen: seen.append(a)
for a in seen:
    leaf='-'
    fs=glob.glob(R+'rebuild/src/compiled/%s/%s/*_%s.cpp'%(a[:2],a[2:4],a))
    if fs:
        for l in open(fs[0],encoding='utf-8',errors='replace'):
            if l.startswith('// Leaf:'): leaf=l[8:].strip(); break
        else: leaf='(landed '+os.path.basename(fs[0])+')'
    b=binds.get(a)
    bs='; '.join('%s(%d)'%(k,v) for k,v in b.most_common(2)) if b else '-'
    print(a, cat.get(a,'-')[:50],'| L:',leaf[:110],'| B:',bs[:200])
