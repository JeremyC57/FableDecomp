# decl.py <entry#> [ident]: for every landed exe caller row of the entry's target (work/xs_all.tsv), print each comment-stripped source
# line naming the identifier (default: the PDB leaf's identifier), with the nearest enclosing class/struct head and access label above it.
# Source-level only (no compile): a reading aid to find every caller still declaring the old access / const / signature. NF314.
import re,sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF314'
R='/home/user/FableDecomp/rebuild/'
ps=open(R+'build_candidates.ps1',encoding='utf-8').read()
def src(a):
    i=ps.find("Address = '%s'"%a)
    if i<0: return None
    m=re.search(r"\bSource = '([^']+)'",ps[i:ps.find("\n    }",i)]); return R+'src/compiled/'+m.group(1) if m else None
L=[l.split('\t') for l in open(A+'/ENTRIES.tsv').read().splitlines()]
n=int(sys.argv[1]); c=L[n]; t=c[0]; pdb=c[6]
idn=sys.argv[2] if len(sys.argv)>2 else (re.match(r'\?\??([^@?]+)@',pdb) or re.match(r'(.*)',pdb)).group(1)
rows=sorted(set(p.split('\t')[3][:8] for p in open(A+'/work/xs_all.tsv') if p.startswith(t) and p.split('\t')[3][8:9]=='L'))
print('#%d %s ident=%s pdb=%s'%(n,t,idn,pdb))
for a in rows:
    f=src(a)
    if not f: print(' ',a,'NOSRC'); continue
    lines=[l.split('//')[0] for l in open(f,encoding='utf-8',errors='replace').read().splitlines()]
    hits=[i for i,l in enumerate(lines) if re.search(r'\b%s\b'%re.escape(idn),l)]
    print(' ',a,f.split('/')[-1])
    for i in hits[:int(sys.argv[3]) if len(sys.argv)>3 else 6]:
        cls=acc=''
        for j in range(i,-1,-1):
            if not acc and re.match(r'\s*(public|protected|private)\s*:',lines[j]): acc=lines[j].strip()
            m=re.match(r'\s*(?:class|struct)\s+(\w+)[^;]*$',lines[j])
            if m: cls=m.group(1); break
        print('    L%d [%s %s] %s'%(i+1,cls,acc,lines[i].strip()[:170]))
