# silook.py <entry#>...: per entry, each non-PDB table line (stand-in binder) with stay-off / T3 flags and the binder source lines that
# name the stand-in identifier or its class (comment-stripped), to classify standin_alias / other entries quickly. NF314.
import re,sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF314'
R='/home/user/FableDecomp/rebuild/'
ps=open(R+'build_candidates.ps1',encoding='utf-8').read()
so=dict(l.split('\t') for l in open(A+'/work/stayoff_why.tsv').read().splitlines())
t3=set(open(A+'/work/t3rows.txt').read().split()); t3m=set(open(A+'/work/t3mention.txt').read().split())
def src(a):
    i=ps.find("Address = '%s'"%a)
    if i<0: return None
    m=re.search(r"\bSource = '([^']+)'",ps[i:ps.find("\n    }",i)]); return R+'src/compiled/'+m.group(1) if m else None
T={}
for l in open(R+'oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: T.setdefault(r[1].lower(),[]).append((r[0],r[2]))
L=[l.split('\t') for l in open(A+'/ENTRIES.tsv').read().splitlines()]
for n in map(int,sys.argv[1:]):
    c=L[n]; t=c[0]; pdb=c[6]
    print('#%d %s pdb=%s self=%s'%(n,t,pdb[:150],c[7][:60]))
    for s,w in T.get(t,[]):
        if s==pdb: continue
        fl=(' SO:'+so[w] if w in so else '')+(' T3' if w in t3 else (' T3M' if w in t3m else ''))
        print('  SI %s %s%s'%(w,s[:150],fl))
        f=src(w)
        if not f: print('     NOSRC'); continue
        m=re.match(r'\?+\$?([^@]+)@([^@]+)@',s) or re.match(r'\?+([^@]+)@',s)
        ids=[x for x in (m.groups() if m else []) if x and not x.startswith('?')]
        lines=[l.split('//')[0] for l in open(f,encoding='utf-8',errors='replace').read().splitlines()]
        k=0
        for i,l in enumerate(lines):
            if any(re.search(r'\b%s\b'%re.escape(x),l) for x in ids):
                print('     L%d %s'%(i+1,l.strip()[:160])); k+=1
                if k>=int(sys.argv[0:1] and 5): break
