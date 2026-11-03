import sys,re,collections
dis=sys.argv[1]
R='/home/user/FableDecomp/'
binds=collections.defaultdict(collections.Counter)
for l in open(R+'rebuild/oracles/reloc_bindings.tsv'):
    p=l.rstrip('\n').split('\t')
    if len(p)>=3: binds[p[1].lower()][p[0]]+=1
man={}
for l in open(R+'rebuild/manifest/functions.tsv'):
    p=l.split('\t')
    man[p[0].lower()]=(p[1],p[2])
seen=[]
for l in open(dis):
    m=re.search(r'(call|push|mov .*|fld|fcomp|fsub|fdivr|fmul|fadd|fild|fstp|fld|cmp)\s.*?(?:ptr \[)?0x([0-9a-f]{6,8})\]?',l)
    if m:
        a=m.group(2).zfill(8)
        if a not in seen: seen.append(a)
for a in seen:
    b=binds.get(a)
    bs='; '.join('%s(%d)'%(k,v) for k,v in b.most_common(3)) if b else '-'
    mm=man.get(a,('-','-'))
    print(a, mm[0][:70], '|', bs[:300])
