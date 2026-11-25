import json,sys
t=json.load(open(sys.argv[1])); k=0; lim=int(sys.argv[2]) if len(sys.argv)>2 else 14
for e in t['events']:
    if e[0]=='dag':
        k+=1; print('DAG',k,len(e[1]), 'noops', [n['idx'] for n in e[1] if n['op']=='0x269'])
        if k==1:
            for n in e[1][8:14]: print('   %3d %-8s fl%-3d u%d prio %x succ(last) %s'%(n['idx'],n['op'],n['fl'],n['unit'],n['prio'],' '.join('%d/%d'%(s[0],s[1]) for s in n['succ'][-5:])))
    elif e[0]=='cyc' and k==1 and 7<=e[1]<lim: print(' cyc',e[1],[(n['op'],n['idx'],hex(n['prio']),n['early']) for n in e[3]])
    elif e[0]=='pick' and (k==2 or (k==1 and e[2]['idx']<26)) : print('   pick',k,'slot',e[1],e[2]['op'],e[2]['idx'],'early',e[2]['early'])
