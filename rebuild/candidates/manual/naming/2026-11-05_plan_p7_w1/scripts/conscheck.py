# conscheck.py W [--prior W0 ...] : one target, one spelling. Reads the gate's `changes` column of wave W (every relocation whose
# symbol changed: `target: old -> new`, and `DEF old -> new` for the row's own symbol), applies the prior waves' changes to the
# index first, and reports per row:
#   OK          every new spelling is (a) already a spelling of that target in the index (the majority or any), or (b) replaces the
#               old spelling completely (every index binder of the old spelling is respelled in this or a prior wave), and the new
#               spelling is bound to no other target / defined at no other address;
#   OK(pdb)     the new spelling is a PDB symbol (Ego_r / FableWin name set) replacing one that is not: the old binders left are
#               listed (CALLERS.tsv) for the coordinator's respell at landing (EXTRA_RULES 2026-11-05 / phase 5 accept_pdb_majority);
#   FLAG <why>  otherwise (the wave would add a spelling to a target that keeps others, or move a name across targets).
# Also lists, per target touched, the spellings left after the wave. Writes W/conscheck.tsv and W/targets_after.tsv.
import sys,os,re,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__))); import wavelib as L
W=sys.argv[1]; prior=[]
if '--prior' in sys.argv: prior=sys.argv[sys.argv.index('--prior')+1:]
E,F=L.pdb(); T,callers=L.index_binds(); ANON=re.compile(r'\?A0x[0-9a-f]{8}')
def_at={}
for a,r in L.cache().items():
    if r.get('sym'): def_at.setdefault(r['sym'],a)
def changes(Wd):
    out=[]
    for r in L.tsv(Wd+'/gate.tsv'):
        if r['verdict']!='PASS': continue
        for ch in filter(None,[x.strip() for x in r['changes'].split(' | ')]):
            m=re.match(r'^([0-9a-f]{8}): (\S+) -> (\S+)$',ch)
            if m:
                if ANON.sub('?A',m.group(2))==ANON.sub('?A',m.group(3)): continue   # `?A0x<hash>` anonymous-namespace churn: the gate compiles from x.cpp
                out.append((r['address'],m.group(1),m.group(2),m.group(3))); continue
            m=re.match(r'^DEF (\S+) -> (\S+)$',ch)
            if m: out.append((r['address'],'DEF',m.group(1),m.group(2)))
    return out
# the state after the prior waves: per (target,symbol) the binders left; per target the spellings
left=collections.defaultdict(set)
for (t,s),cs in callers.items(): left[(t,s)]=set(cs)
added=collections.defaultdict(set)
for Wd in prior:
    for a,t,o,n in changes(Wd):
        if t=='DEF': continue
        left[(t,o)].discard(a); added[(t,n)].add(a)
ch=changes(W); rows=collections.defaultdict(list)
for a,t,o,n in ch: rows[a].append((t,o,n))
res=[]; touched=set()
for a in sorted(rows):
    why=[]; pdbok=[]
    for t,o,n in rows[a]:
        if t=='DEF':
            if n in def_at and def_at[n]!=a: why.append('own symbol %s is defined at %s'%(n[:60],def_at[n]))
            continue
        touched.add(t)
        others=T.get(n,{})
        other_t=set(x for x in L.index_binds.__defaults__ or []) if False else None
        present=(n in T[t]) or bool(added[(t,n)])
        old_left=set(left[(t,o)])-set(x for x,tt,oo,nn in ch if tt==t and oo==o)   # binders of the old spelling not respelled by this wave
        if not present and old_left:
            # the PDB rule (EXTRA_RULES 2026-11-05, phase 5 accept_pdb_majority): a new spelling that is a real PDB symbol replacing one
            # that is not is the right name; the old binders are listed in CALLERS.tsv for the coordinator's respell at landing
            if (n in E or n in F) and not (o in E or o in F): pdbok.append('%s: %s replaces the non-PDB %s (%d binder(s) left: %s)'%(t,n[:50],o[:50],len(old_left),','.join(sorted(old_left)[:4])))
            else: why.append('%s: new spelling %s not at target; old %s keeps %d binder(s) %s'%(t,n[:50],o[:50],len(old_left),','.join(sorted(old_left)[:4])))
        bound_elsewhere=[tt for tt in T if n in T[tt] and tt!=t]
        if bound_elsewhere: why.append('%s: %s is bound in the index to %s'%(t,n[:50],','.join(bound_elsewhere[:3])))
        if n in def_at and def_at[n]!=t and not n.startswith('??_7'): why.append('%s: %s is defined at %s'%(t,n[:50],def_at[n]))
    res.append((a,'FLAG' if why else ('OK(pdb)' if pdbok else 'OK'),'; '.join(why+pdbok)))
with open(W+'/conscheck.tsv','w') as o:
    o.write('address\tverdict\twhy\n')
    for r in res: o.write('\t'.join(r)+'\n')
with open(W+'/targets_after.tsv','w') as o:
    o.write('target\tspellings_after (symbol:count)\n')
    for t in sorted(touched):
        c=collections.Counter()
        for (tt,s),cs in left.items():
            if tt==t: c[s]+=len(cs-set(a for a,x,oo,nn in ch if x==t and oo==s))
        for (tt,s),cs in added.items():
            if tt==t: c[s]+=len(cs)
        for a,x,oo,nn in ch:
            if x==t: c[nn]+=1
        o.write(t+'\t'+' || '.join('%s:%d'%(s,n) for s,n in c.most_common() if n>0)+'\n')
cnt=collections.Counter(r[1] for r in res); print('conscheck',W,dict(cnt),'targets touched',len(touched))
for r in res:
    if r[1]=='FLAG': print(' ',r[0],r[2][:200])
