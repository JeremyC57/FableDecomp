import re,collections,sys
S=sys.argv[1]
norm=lambda n: re.sub(r'\b(class|struct|enum) ','',n).replace(' ','')
bc=open('rebuild/build_candidates.ps1',encoding='utf-8').read()
landed=set(a.lower() for a in re.findall(r"Address = '([0-9a-fA-F]{8})'",bc))
cat={}
for l in open('rebuild/oracles/auto-re-candidates.tsv',encoding='latin-1').read().splitlines()[1:]:
    s=l.split('\t'); 
    if len(s)>2: cat.setdefault(norm(s[1]),[]).append((int(s[0],16),int(s[2]) if s[2].isdigit() else 0))
P=[l.split('\t') for l in open(S+'/procs2.tsv',encoding='latin-1').read().splitlines()]
fab=[p for p in P if '\\fable\\' in p[3].lower() or '\\bbblibrary' in p[3].lower() or 'fable' in p[3].lower()]
print('360 Fable-file procs',len(fab),'(of',len(P),')')
uniq={}
for p in fab:
    uniq.setdefault(norm(p[0]),[]).append(p)
m=[(k,v) for k,v in uniq.items() if k in cat]
print('distinct 360 names',len(uniq),'matching a TLC catalog name',len(m))
addrs=[a for k,v in m for a,_ in cat[k]]
un=[a for a in addrs if '%08x'%a not in landed]
print('TLC catalog functions covered',len(addrs),'of which unlanded',len(un))
# T2 order test: per source file, TLC address order vs 360 line order (unique names only)
pairs=same=0; files=collections.defaultdict(list)
for k,v in m:
    if len(v)==1 and len(cat[k])==1 and v[0][4]!='0':
        files[v[0][3].lower()].append((int(v[0][4]),cat[k][0][0]))
nf=0; good=0
for f,L in files.items():
    if len(L)<4 or not f.endswith('.cpp'): continue
    L.sort(); nf+=1
    s=sum(1 for i in range(len(L)-1) if L[i+1][1]>L[i][1]); pairs+=len(L)-1; same+=s
    span=max(a for _,a in L)-min(a for _,a in L)
print('order test: .cpp files',nf,'adjacent pairs',pairs,'same order as TLC',same,'%.1f%%'%(100*same/max(pairs,1)))
# unlanded TLC funcs with params/locals names
withloc=sum(1 for k,v in m for a,_ in cat[k] if '%08x'%a not in landed and v[0][5])
print('unlanded TLC functions that get param/local names',withloc)
