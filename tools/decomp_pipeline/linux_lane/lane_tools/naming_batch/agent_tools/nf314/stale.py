# stale.py: for every non-PDB table line of an ENTRIES target, does the witness row's source still name the stand-in identifier?
# A witness whose comment-stripped source lacks it is a stale-line candidate (confirm with a HEAD object read before any delete). NF314.
import re
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF314'
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
for n in range(1,len(L)):
    t=L[n][0]; P=L[n][6]
    for s,w in T.get(t,[]):
        if s==P: continue
        m=re.match(r'[?@_]+\$?([A-Za-z_]\w*)',s)
        if not m: continue
        idn=m.group(1)
        f=src(w)
        if not f: continue
        code='\n'.join(x.split('//')[0] for x in open(f,encoding='utf-8',errors='replace').read().splitlines())
        if not re.search(r'\b%s\b'%re.escape(idn),code): print('%d\t%s\t%s\t%s\tident=%s'%(n,t,w,s[:120],idn))
