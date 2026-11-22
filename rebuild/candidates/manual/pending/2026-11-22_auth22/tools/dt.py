import json,re,sys
E=json.load(open(sys.argv[1]))
idx=[i for i,e in enumerate(E) if e[0]=='pipe']
last=idx[-1]; prev=idx[-2]
D=[e[1] for e in E[prev+1:last] if e[0]=='d']
pat=re.compile(sys.argv[2] if len(sys.argv)>2 else r'push_back@\?\$list|CreateRelevant|\?\?0CThingAICreatureInit|SetHomeBuilding|push_back@\?\$vector@PAVCThingAICreature')
n=0
for k,d in enumerate(D):
    if pat.search(d['n']):
        nm=re.sub(r'@.*','',d['n'].lstrip('?'))[:22]
        if 'push_back@?$list' in d['n']: nm='list_pb'
        if 'push_back@?$vector' in d['n']: nm='vec_pb'
        print("%4d %-22s g%6d c1 %4d f%7.3f pen %3s sc %4s r%s"%(k,nm,d['g'],d['c1'],d['flo']/1e8,d['pen'],d.get('sc','?'),d['r']))
print("total decisions",len(D),"final g",D[-1]['g'])
