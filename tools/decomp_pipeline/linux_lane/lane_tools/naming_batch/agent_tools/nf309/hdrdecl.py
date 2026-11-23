# hdrdecl.py <census file>: for each CENSUS_PORT entry, does any stand-in binder include a shared header that declares the PDB member
# identifier (NF299 Q4: such entries must be opened by hand)? Prints HDR_HIT lines with the header and line. NF309.
import re,sys,os
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF309'
R='/home/user/FableDecomp/rebuild/'
ps=open(R+'build_candidates.ps1',encoding='utf-8').read()
def src(a):
    i=ps.find("Address = '%s'"%a)
    if i<0: return None
    m=re.search(r"\bSource = '([^']+)'",ps[i:ps.find("\n    }",i)]); return R+'src/compiled/'+m.group(1) if m else None
L=[l.split('\t') for l in open(A+'/ENTRIES.tsv').read().splitlines()]
for l in open(sys.argv[1]):
    n,t,v,rest=(l.rstrip('\n').split('\t')+[''])[:4]
    if not v.startswith('CENSUS_PORT'): continue
    P=L[int(n)][6]; m=re.match(r'\?\??([^@?]+)@',P); idn=m.group(1) if m else None
    cm=re.match(r'\?+[^@]+@([^@?]+)@',P); cls=cm.group(1) if cm else None
    hits=[]
    for b in [x.split(':')[0] for x in rest.split()]:
        f=src(b)
        if not f: continue
        for inc in re.findall(r'#include\s+"(engine/[^"]+)"',open(f,encoding='utf-8',errors='replace').read()):
            h=R+'include/'+inc
            if not os.path.exists(h): continue
            for k,hl in enumerate(open(h,encoding='utf-8',errors='replace')):
                if idn and re.search(r'\b%s\s*\('%re.escape(idn),hl.split('//')[0]): hits.append('%s:%s:%d'%(b,inc,k+1)); break
    print('%s\t%s\t%s\t%s'%(n,t,'HDR_HIT' if hits else 'no_hdr',' '.join(hits[:4])))
