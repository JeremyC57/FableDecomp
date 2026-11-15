import json,os,re,collections
S=os.environ['S']
R='/home/user/FableDecomp'; P=R+'/rebuild/candidates/manual/pending/'; INC=R+'/rebuild/include/'
by=json.load(open(S+'/lc/staged.json'))
items=collections.defaultdict(list); bad=[]
def rel_of(path,fname):
    for k in ('rebuild/include/','/hdr/','/include/','/inc/'):
        if k in path: return path.split(k,1)[1]
    return None
for d in by:
    h=P+d+'/hdrdiff'
    if not os.path.isdir(h): continue
    for f in sorted(os.listdir(h)):
        fp=h+'/'+f
        if f.endswith('.diff'):
            t=open(fp,encoding='latin-1').read()
            m=re.search(r'^\+\+\+ (\S+)',t,re.M)
            rel=rel_of(m.group(1),f) if m else None
            if not rel or not os.path.exists(INC+rel):
                # try --- line / filename forms
                m2=re.search(r'^--- (\S+)',t,re.M); r2=rel_of(m2.group(1),f) if m2 else None
                cands=[r2, f[:-5].replace('engine_','engine/',1).replace('stlport_','stlport/',1), 'engine/'+f[:-5], f[:-5], 'stlport/'+f[:-5]]
                rel=next((c for c in cands if c and os.path.exists(INC+c)),None)
            if not rel: bad.append((d,f)); continue
            items[rel].append(['diff',d,fp])
        elif f.startswith('NEW_'):
            n=f[4:]; n2=n.replace('engine_','engine/',1).replace('stlport_','stlport/',1)
            if '/' not in n2: n2=('engine/'+n2) if not n2.startswith('fable_') else n2
            items[n2].append(['new',d,fp])
        elif f.endswith('.md'): pass
        else: bad.append((d,f))
json.dump(items,open(S+'/lc/items.json','w'),indent=1)
print('targets',len(items),'diffs',sum(1 for v in items.values() for x in v if x[0]=='diff'),'new',sum(1 for v in items.values() for x in v if x[0]=='new'))
print('unresolved',bad)
newt=[k for k,v in items.items() if any(x[0]=='new' for x in v)]
print('new targets',len(newt), [k for k in newt if os.path.exists(INC+k)])
print('both new and diff',[k for k,v in items.items() if len({x[0] for x in v})>1])
