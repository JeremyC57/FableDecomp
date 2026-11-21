# keys_an.py k.json lowA lowB : find the smallest tree nodes whose subtree has leaves with both hash words; print subtree
import json,sys
ev=json.load(open(sys.argv[1]))['events']
A=int(sys.argv[2],16); B=int(sys.argv[3],16)
lat={}; nodes=[]
fn=None
for e in ev:
    if e[0]=='pipe': fn=e[1]; continue
    if e[0]!='k': continue
    _,addr,kind,key,ch=e
    rec={'addr':addr,'kind':kind,'key':key,'ch':[lat.get(c) for c in ch],'fn':fn}
    def leaves(r,acc,d=0):
        if r is None or d>40: return acc
        if not r['ch']: acc.add(r['key']&0xffff)
        for c in r['ch']: leaves(c,acc,d+1)
        return acc
    rec['lv']=leaves(rec,set())
    lat[addr]=rec; nodes.append(rec)
def show(r,ind=0,d=0):
    if r is None: print(" "*ind+"?"); return
    print(" "*ind+"k%02x key %08x (su %d size %d h %04x)"%(r['kind'],r['key'],r['key']>>24,(r['key']>>16)&0xff,r['key']&0xffff))
    if d<12:
        for c in r['ch']: show(c,ind+2,d+1)
hit=[r for r in nodes if A in r['lv'] and B in r['lv']]
seen=set()
for r in hit:
    if any(c is not None and A in c['lv'] and B in c['lv'] for c in r['ch']): continue
    if r['addr'] in seen: continue
    seen.add(r['addr']); print("== fn",r['fn']); show(r)
    if len(seen)>=int(sys.argv[4] if len(sys.argv)>4 else 3): break
