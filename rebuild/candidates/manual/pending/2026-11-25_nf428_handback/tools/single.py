import json,sys,re,collections
A=sys.argv[1]; R='/home/user/FableDecomp'
r=json.load(open(A+'/status/rec.json')); occ=json.load(open(A+'/evidence/occ.json'))
tu=json.load(open(A+'/evidence/tu_occ.json')); c=json.load(open(A+'/evidence/tu_cand.json'))
FAM={'?Reset@AutoTinyCountedHandle@@QAEXXZ','?Release@CIVCP@@QAEXXZ'}
# Ego_r object streams
eg=collections.defaultdict(list)
for l in open(R+'/ghidra_out/egor_obj_functions.tsv',errors='replace'):
    f=l.rstrip('\n').split('\t')
    if len(f)>=7: eg[f[0].lower().split('\\')[-1]].append(f[5])
out=[];why=collections.Counter()
for o,v in sorted(tu.items()):
    cd=c.get(o,[])
    if len(v)!=2 or len(cd)!=2: continue
    names=sorted(n for i,n,s in cd)
    m=[re.match(r'^(\?\?1|\?Release@)\?\$(CDefPointer|CIVCountedPointer)@(.*)@@QAE(@|X)XZ$',n) for n in names]
    if not all(m): why['nomatch']+=1; continue
    ts={x.group(3) for x in m}; kinds={x.group(1) for x in m}
    if len(ts)!=1 or kinds!={'??1','?Release@'}: why['twoT']+=1; continue
    T=ts.pop(); tmpl=m[0].group(2)
    rel=[n for n in names if n.startswith('?Release')][0]; dtor=[n for n in names if n.startswith('??1')][0]
    fam=[r[a]['self_leaf'] in FAM for a in v]
    # ego_r second source: object basename
    ob=o.lower().split('\\')[-1]
    # readable T: strip $$CB V ... @@
    tr=T
    tname=re.sub(r'^\$\$CB','',T); tname=tname[1:] if tname[0] in 'VU' else tname
    parts=tname.split('@'); simple=parts[0]
    egh=[n for n in eg.get(ob,[]) if re.search(r'\b'+re.escape(simple)+r'\b',n)]
    out.append(dict(obj=o,addrs=v,shapes=[occ[a][0] for a in v],tmpl=tmpl,T=T,release=rel,dtor=dtor,fam=fam,ego=egh[:4],nego=len(egh)))
json.dump(out,open(A+'/evidence/single.json','w'),indent=1)
print(len(out),why)
print('both fam',sum(all(x['fam']) for x in out),'with ego',sum(1 for x in out if x['nego'] and all(x['fam'])))
for x in out:
    if not x['nego'] or not all(x['fam']): print('NOEGO/NONFAM',x['obj'],x['addrs'],x['T'],x['fam'])
