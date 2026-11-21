import json,sys
ev=json.load(open(sys.argv[1]))['events']
A=int(sys.argv[2],16); B=int(sys.argv[3],16)
lat={}; i=0
for e in ev:
    i+=1
    if e[0]!='k': continue
    _,addr,kind,key,ch=e
    rec={'key':key,'ch':[lat.get(c) for c in ch]}
    def lv(r,acc,d=0):
        if r is None or d>40: return acc
        if not r['ch']: acc.add(r['key']&0xffff)
        for c in r['ch']: lv(c,acc,d+1)
        return acc
    rec['lv']=lv(rec,set()); lat[addr]=rec
    if A in rec['lv'] and B in rec['lv'] and not any(c and A in c['lv'] and B in c['lv'] for c in rec['ch']):
        print(i, "%08x"%key, ["%08x"%c['key'] if c else None for c in rec['ch']], [sorted("%04x"%x for x in c['lv']) if c else None for c in rec['ch']])
