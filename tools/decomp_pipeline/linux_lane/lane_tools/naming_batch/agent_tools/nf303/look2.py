# look2.py <entry#>...: compact look: per entry the PDB leaf, stand-in identifiers, exe-calling landed rows, and per row that names a
# stand-in identifier in code: the count of code lines naming it and its first declaration-looking line (trimmed).
import re,sys,subprocess
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF303'
R='/home/user/FableDecomp/rebuild/'
ps=open(R+'build_candidates.ps1',encoding='utf-8').read()
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
    c=L[n]; t=c[0]; pdb=c[6]; ids={}
    for s,w in T.get(t,[]):
        if s!=pdb:
            m=re.match(r'\?([^@?]+)@',s)
            if m: ids.setdefault(m.group(1),[]).append(w)
    xs=subprocess.run(['python3',A+'/tools/xsites.py',t],capture_output=True,text=True).stdout.split('\n')
    rows=sorted(set(x.split('\t')[3][:8] for x in xs if x and x.split('\t')[3]!='-' and x.split('\t')[3][8]=='L'))
    un=len([x for x in xs if x and (x.split('\t')[3]=='-' or x.split('\t')[3][8]!='L')])
    print('#%d %s PDB=%s ids=%s xs_rows=%d unl=%d'%(n,t,pdb[:90],dict((k,v[:3]) for k,v in ids.items()),len(rows),un))
    for b in sorted(set(rows)|set(w for v in ids.values() for w in v if len(w)==8)):
        f=src(b)
        if not f: continue
        code=[l.split('//')[0] for l in open(f,encoding='utf-8',errors='replace').read().splitlines()]
        hits=[(i,l) for i,l in enumerate(code,1) if any(re.search(r'\b%s\b'%re.escape(x),l) for x in ids)]
        if not hits: continue
        d=[h for h in hits if re.search(r'(;|\{)\s*$',h[1]) and not re.search(r'(->|\.)\s*\w+\s*\(',h[1])] or hits
        i,l=d[0]; x=[k for k in ids if k in l]; j=l.find(x[0]) if x else 0
        print('   %s n=%d L%d: %s'%(b,len(hits),i,l[max(0,j-90):j+90].strip()))
