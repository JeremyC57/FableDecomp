# keytree.py k.json eventindex : print the tree at that k event with each node's derived opcode term
import json,sys
ev=json.load(open(sys.argv[1]))['events']; tgt=int(sys.argv[2])
lat={}
for i,e in enumerate(ev,1):
    if e[0]!='k': continue
    _,addr,kind,key,ch=e
    rec={'key':key,'kind':kind,'ch':[lat.get(c) for c in ch]}; lat[addr]=rec
    if i==tgt: root=rec; break
def show(r,ind=0):
    h=r['key']&0xffff
    s=sum(((c['key']&0xffff)<<(j&7)) for j,c in enumerate(r['ch']))
    op=(h-s)&0xffff if r['ch'] else None
    print(" "*ind+"k%02x %08x  h=%04x%s"%(r['kind'],r['key'],h,"  op=%x"%op if op is not None else ""))
    for c in r['ch']: show(c,ind+2)
show(root)
