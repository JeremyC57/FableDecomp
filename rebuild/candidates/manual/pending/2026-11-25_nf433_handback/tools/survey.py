import csv, re, collections, subprocess, os, sys
R='/home/user/FableDecomp'; W=sys.argv[1]
rows=[l.rstrip('\n').split('\t') for l in open(W+'/open_cgsi.tsv')]
self={}
for l in open(R+'/rebuild/oracles/self_symbols.tsv').read().splitlines()[1:]:
    f=l.split('\t'); self[f[0].lower()]=f[1] if len(f)>1 else ''
bind=collections.defaultdict(list)
for l in open(R+'/rebuild/oracles/reloc_bindings.tsv').read().splitlines()[1:]:
    q=l.split('\t'); bind[q[1].lower()].append((q[0],q[2]))
rtti=collections.defaultdict(set)
for l in open(R+'/ghidra_out/labels_rtti_port.tsv'):
    q=l.rstrip('\n').split('\t'); rtti[q[0].lower()].add(q[1]) if len(q)>1 else None
bysym=collections.defaultdict(set)
for a,s in self.items(): bysym[s].add(a)
cat={}
for l in open(R+'/rebuild/oracles/auto-re-candidates.tsv').read().splitlines()[1:]:
    q=l.split('\t'); cat[q[0].lower()]=q
out=open(W+'/survey.tsv','w')
for r in rows:
    a=r[0]; st=r[1]
    src=R+'/rebuild/src/compiled/'+r[10] if r[10] else ''
    t=open(src).read() if src and os.path.exists(src) else ''
    nloc=len(re.findall(r'^\s*(?:class|struct)\s+\w+[^;]*\{',t,re.M))
    size=len(bytes.fromhex(cat[a][3])) if a in cat and len(cat[a])>3 and re.fullmatch('[0-9a-fA-F]*',cat[a][3] or 'x') else -1
    oself=self.get(a,'')
    callers=[c for c in bind.get(a,[]) if c[0]!=oself]
    same_old=[c for c in bind.get(a,[]) if c[0]==oself]
    tgt=';'.join(sorted(rtti.get(a,[])))
    out.write('\t'.join([a,st,str(size),str(len(t.splitlines())),str(nloc),str(len(same_old)),str(len(callers)),oself[:90],tgt[:120]])+'\n')
