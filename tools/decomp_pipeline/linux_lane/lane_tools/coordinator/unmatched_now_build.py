import json,csv,re,glob,os,sys,collections
csv.field_size_limit(sys.maxsize)
S=sys.argv[1]; d=json.load(open(S+'/um/sets.json'))
known=set(d['known']); gen=set(d['genuine']); orig=set(d['original']); cat=set(d['catalog']); kinds=d['kinds']
A=lambda s:s.strip().lower().replace('0x','').zfill(8)
fn={}
for r in csv.DictReader(open('rebuild/manifest/functions.tsv',encoding='utf-8-sig'),delimiter='\t'):
    fn[A(r['address'])]=r
um={}
for r in csv.DictReader(open('rebuild/candidates/manual/UNMATCHED_ROWS.tsv',encoding='utf-8'),delimiter='\t'):
    um[A(r['address'])]=r
pk={}
for r in csv.DictReader(open('rebuild/candidates/manual/PARKED_INDEX.tsv',encoding='utf-8'),delimiter='\t'):
    pk[A(r['addr'])]=r
res=collections.defaultdict(list)
for p in sorted(glob.glob('rebuild/candidates/manual/pending/*/RESERVED.tsv')):
    tag=p.split('/')[-2]
    for r in csv.reader(open(p,encoding='utf-8',errors='replace'),delimiter='\t'):
        if r and re.fullmatch(r'(0x)?[0-9a-fA-F]{8}',r[0]): res[A(r[0])].append((tag,' '.join(r[1:])[:160]))
claims=collections.defaultdict(list)
for r in csv.reader(open('rebuild/candidates/manual/COORDINATION_CLAIMS.tsv',encoding='utf-8'),delimiter='\t'):
    if len(r)>4:
        for x in re.findall(r'0x([0-9a-fA-F]{8})',r[2]+' '+r[5] if len(r)>5 else r[2]): claims[A(x)].append((r[0],r[1],r[4]))
locks={}
for o in glob.glob(S+'/w22/nflocks/*/owner'):
    w=open(o).read().strip()
    if w!='released': locks[A(o.split('/')[-2])]=w
sk={A(r['address']):r for r in csv.DictReader(open('rebuild/candidates/manual/SKIPPED_ROWS.tsv',encoding='utf-8'),delimiter='\t')}
bf={A(r['address']):r for r in csv.DictReader(open('rebuild/candidates/manual/BOUNDS_FIX.tsv',encoding='utf-8'),delimiter='\t')}
rows=[]
for a in sorted(known-gen):
    f=fn.get(a,{}); u=um.get(a,{}); p=pk.get(a,{})
    name=(f.get('module','')+'::' if f.get('module','') not in ('','_global') else '')+f.get('name','')
    if a in cat:
        st='landed, not counted as genuine ('+kinds.get(a,'?')+')'
        if a in d['baked_string']: st='landed, baked string address (not counted until the literal is spelled)'
        if a in d['baked_address']: st='landed, baked data address (not counted until spelled)'
    else:
        st=''
        if a in locks: st='locked by '+locks[a]
        cl=claims.get(a,[]); oc=[cl[-1]] if cl and cl[-1][2] not in ('done','released') else []
        if not st and oc: st='claimed by '+oc[-1][1]+' ('+oc[-1][0]+')'
        live=[x for x in res.get(a,[]) if 'landed' not in x[1].split(' ')[-3:] and not re.search(r'\blanded\b|\breleased\b|returned',x[1],re.I)]
        if not st and live: st='reserved: '+live[-1][0]
        if not st and u.get('status'): st='UNMATCHED: '+u['status']
        if not st and p.get('status'): st='PARKED_INDEX: '+p['status']
        if not st and a in sk: st='skipped row: '+sk[a].get('status','')+' '+sk[a].get('kind','')
        if not st and a in bf: st='bounds fix: '+bf[a].get('action','')
        if not st: st='no record (never attempted?)'
    why=(u.get('best_grade_on_record','') or p.get('last_lens','')) 
    note=(p.get('diagnosis','') or p.get('residue_class','') or '')[:120]
    rv=res.get(a,[])
    rows.append(dict(address=a,name=name,original='original' if a in orig else (f.get('agent_source','') or 'later'),length=u.get('length_estimate',''),
        status=st,best_grade=why,reserved_in=';'.join(x[0] for x in rv[-2:]),reserve_note=(rv[-1][1] if rv else ''),note=note))
k=['address','name','original','length','status','best_grade','reserved_in','reserve_note','note']
with open(S+'/um/UNMATCHED_NOW.tsv','w') as o:
    o.write('\t'.join(k)+'\n'); [o.write('\t'.join(str(r[x]).replace('\t',' ') for x in k)+'\n') for r in rows]
c=collections.Counter(re.sub(r' \(\d{4}-.*','',re.sub(r'^(reserved: \S+?_)(?:\w+)$',r'\1',r['status']))[:90] for r in rows)
for kk,v in c.most_common(): print(v,kk)
print(len(rows), 'original',sum(1 for r in rows if r['original']=='original'))
