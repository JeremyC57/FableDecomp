import sys,collections
d=collections.defaultdict(set)
for l in open(sys.argv[1]).read().splitlines()[1:]:
    s=l.split('\t')
    if len(s)>=2: d[s[0]].add(s[1].lower())
m=[k for k,v in d.items() if len(v)>1]; print(len(m)); [print(k,sorted(d[k])) for k in m[:5]]
