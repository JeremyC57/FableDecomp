import json,sys,re
E=json.load(open(sys.argv[1])); pat=re.compile(sys.argv[2] if len(sys.argv)>2 else r'LowerBound|CParticleUpdateParameters|insert_overflow|uninitialized_copy|\?\?1\?\$vector|Insert@|GetTC|\?Calc@')
cur=None; buf=[]
for e in E:
    if e[0]=='pipe':
        if buf: print('== in', cur[:80]); print('\n'.join(buf)); buf=[]
        cur=e[1]; continue
    d=e[1]
    if pat.search(d['n']): buf.append(f"  {d['n'][:90]:90} sz{d['sz']:4} c1 {d['c1']:3} f{d['flo']/1e8:7.3f} sc {d.get('sc','?')} r{d['r']}")
# decisions belong to the pipe that FOLLOWS? print trailing
if buf: print('== trailing (before end)'); print('\n'.join(buf))
