# look.py <entry#>...: per entry, its table lines, the exe call sites of the target (xsites rows) and, per landed binder, the code lines
# naming any stand-in identifier of the entry (comment-stripped), to classify the respell before any compile.
import re,sys,subprocess,os
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF309'
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
    c=L[n]; t=c[0]; pdb=c[6]
    print('#### #%d %s pdb=%s self=%s'%(n,t,pdb,c[7][:60]))
    ids=set()
    for s,w in T.get(t,[]):
        print('  tbl %s %s %s'%('PDB' if s==pdb else 'SI ',w,s[:140]))
        if s!=pdb:
            m=re.match(r'\?([^@?]+)@',s)
            if m: ids.add(m.group(1))
    xs=subprocess.run(['python3',A+'/tools/xsites.py',t],capture_output=True,text=True).stdout.split('\n')
    rows=sorted(set(x.split('\t')[3][:8] for x in xs if x and x.split('\t')[3]!='-' and x.split('\t')[3][8]=='L'))
    print('  xsites %d sites, landed rows %s, unlanded %d'%(len([x for x in xs if x]),' '.join(rows),len([x for x in xs if x and (x.split('\t')[3]=='-' or x.split('\t')[3][8]!='L')])))
    for b in sorted(set(rows)|set(w for s,w in T.get(t,[]) if len(w)==8)):
        f=src(b)
        if not f: continue
        code=[l.split('//')[0] for l in open(f,encoding='utf-8',errors='replace').read().splitlines()]
        for i,l in enumerate(code,1):
            if any(re.search(r'\b%s\b'%re.escape(x),l) for x in ids):
                j=min(l.find(x) for x in ids if x in l); print('    %s:%d %s'%(b,i,l[max(0,j-150):j+120].strip()))
