import json,collections,sys,bisect
R='/home/user/FableDecomp'
A=sys.argv[1]
r=json.load(open(A+'/status/rec.json'))
occ=json.load(open(A+'/evidence/occ.json'))
anch={int(k,16):v for k,v in json.load(open(A+'/evidence/anchors.json')).items()}
def plain(a):
    n=r['%08x'%a]['pdb_spelling']
    return '?$' not in n and not n.startswith('??_') 
pa=sorted(a for a in anch if plain(a))
res={}; c=collections.Counter()
for k in sorted(occ):
    a=int(k,16); i=bisect.bisect(pa,a)
    lo=pa[i-1] if i else None; hi=pa[i] if i<len(pa) else None
    ol=anch.get(lo); oh=anch.get(hi)
    st='same' if ol==oh else 'diff'
    c[st]+=1
    res[k]=[occ[k][0],'%08x'%lo,ol,'%08x'%hi,oh,st]
json.dump(res,open(A+'/evidence/occ_tu.json','w'),indent=0)
print(c)
objs=collections.Counter(v[2] for v in res.values() if v[5]=='same')
print(len(objs)); print(objs.most_common(15))
