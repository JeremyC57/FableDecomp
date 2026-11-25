import json,sys
t=json.load(open(sys.argv[1])); k=0; pr={}
for e in t['events']:
    if e[0]=='dag': k+=1
    if e[0]=='pick' and k==1: pr[e[2]['idx']]=e[2]
    if e[0]=='cyc' and k==1:
        for n in e[3]: pr.setdefault(n['idx'],n)
d=[e[1] for e in t['events'] if e[0]=='dag']
print('regions',[len(x) for x in d],'noops',[n['idx'] for n in d[0] if n['op']=='0x269'])
for n in d[0][8:16]:
    p=pr.get(n['idx'],n); print('  %3d %-6s u%d prio %4d depth %3d c22 %2d early %d succtail %s'%(n['idx'],n['op'],n['unit'],p['prio']>>10,p['depth'],p['c22'],p['early'],' '.join('%d/%d'%(s[0],s[1]) for s in n['succ'][-4:])))
k=0; o=[]
for e in t['events']:
    if e[0]=='dag': k+=1
    if e[0]=='pick' and k==1 and 8<=e[2]['idx']: o.append('%s#%d'%(e[2]['op'],e[2]['idx']))
print(' order:',' '.join(o[:12]))
