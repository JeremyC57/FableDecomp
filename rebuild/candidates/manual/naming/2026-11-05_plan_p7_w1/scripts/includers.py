# includers.py HEADER_REL... : landed rows (address) whose source includes one of the headers, directly or through the include
# closure of rebuild/include (repo text). Prints addresses, one per line. Plan phase 7.
import sys,os,re
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__))); import wavelib as L
INC=L.ROOT+'/rebuild/include'; want=set(sys.argv[1:])
hdrs={}
for d,_,fl in os.walk(INC):
    for f in fl:
        if f.endswith('.h'):
            rel=os.path.relpath(os.path.join(d,f),INC); hdrs[rel]=set(re.findall(r'#\s*include\s*"([^"]+)"',L.read(os.path.join(d,f))))
closure=set(want); ch=True
while ch:
    ch=False
    for h,incs in hdrs.items():
        if h not in closure and incs&closure: closure.add(h); ch=True
for a,e in L.ent().items():
    p=L.ROOT+'/rebuild/src/compiled/'+e['Source']
    if not os.path.exists(p): continue
    if set(re.findall(r'#\s*include\s*"([^"]+)"',L.read(p)))&closure: print(a)
