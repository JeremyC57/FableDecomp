import json,sys
ev=json.load(open(sys.argv[1])); fn=sys.argv[2] if len(sys.argv)>2 else None
cur='?'
for e in ev:
    if e[0]=='pipe': cur=e[1]; print('== pipe',cur); continue
    if e[0]=='d':
        d=e[1]; print('  %-60s sz=%s f=%.4f pen=%s c0=%s c1=%s sc=%s T=%s %s'%(d['n'][:60],d.get('sz'),d['flo']/1e8+d['fhi']*42.94967296,d.get('pen'),d.get('c0'),d.get('c1'),d.get('sc'),d.get('T'),'EXPAND' if d['r'] else 'call'))
    else: print(' ',e[0],str(e[1:])[:100])
