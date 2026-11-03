# incres.py <src> <incdir>... : resolve #include "..." / <...> recursively (no conditional eval), list missing headers
import sys,re,os
src=sys.argv[1]; dirs=sys.argv[2:]
seen=set(); missing={}
def find(name,cur):
    for d in [cur]+dirs:
        p=os.path.join(d,name)
        if os.path.isfile(p): return p
    return None
def walk(p):
    if p in seen: return
    seen.add(p)
    for l in open(p,errors='replace'):
        m=re.match(r'\s*#\s*include\s*[<"]([^>"]+)[>"]',l)
        if m:
            q=find(m.group(1),os.path.dirname(p))
            if q: walk(q)
            else: missing.setdefault(m.group(1),[]).append(os.path.basename(p))
walk(src)
for k,v in missing.items(): print('MISSING',k,'from',sorted(set(v)))
print(len(seen),'files')
