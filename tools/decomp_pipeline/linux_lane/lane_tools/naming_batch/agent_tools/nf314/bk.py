# bk.py <entry#>...: per entry, every landed exe caller row (xsites) bucketed by what its comment-stripped code names:
#   PDB = the PDB leaf identifier, SI = a table stand-in identifier for this target, NONE = neither; flags SO (stay-off), T3 (queue row).
import re,sys,subprocess
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF314'
R='/home/user/FableDecomp/rebuild/'
ps=open(R+'build_candidates.ps1',encoding='utf-8').read()
so=set(open(A+'/work/stayoff2.txt').read().split()); t3=set(open(A+'/work/t3rows.txt').read().split())
def src(a):
    i=ps.find("Address = '%s'"%a)
    if i<0: return None
    m=re.search(r"\bSource = '([^']+)'",ps[i:ps.find("\n    }",i)]); return R+'src/compiled/'+m.group(1) if m else None
T={}
for l in open(R+'oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: T.setdefault(r[1].lower(),[]).append((r[0],r[2]))
L=[l.split('\t') for l in open(A+'/ENTRIES.tsv').read().splitlines()]
xs={}
for l in open(A+'/work/xs_all.tsv'):
    p=l.rstrip('\n').split('\t'); xs.setdefault(p[0],[]).append(p)
for n in map(int,sys.argv[1:]):
    c=L[n]; t=c[0]; pdb=c[6]
    m=re.match(r'\?([^@?]+)@',pdb); pid=m.group(1) if m else None
    ids={}
    for s,w in T.get(t,[]):
        if s!=pdb:
            m=re.match(r'\??\$?([^@?]+)@',s.lstrip('?'))
            m=re.match(r'\?([^@?]+)@',s) or re.match(r'@([^@]+)@',s)
            if m: ids.setdefault(m.group(1),[]).append(w)
    rows={}
    for p in xs.get(t,[]):
        r=p[3]
        if r=='-': rows.setdefault('-unlisted',0); continue
        rows.setdefault(r[:9],0); rows[r[:9]]+=1
    out=[]
    for r in sorted(rows):
        if r=='-unlisted' or r[8]!='L': out.append('%s:UNLANDED'%r[:8]); continue
        a=r[:8]; f=src(a); fl=('SO' if a in so else '')+('T3' if a in t3 else '')+('SELF' if a==t else '')
        if not f: out.append('%s:NOSRC'%a); continue
        code='\n'.join(l.split('//')[0] for l in open(f,encoding='utf-8',errors='replace').read().splitlines())
        b=[]
        if pid and re.search(r'\b%s\b'%re.escape(pid),code): b.append('PDB')
        si=[k for k in ids if re.search(r'\b%s\b'%re.escape(k),code)]
        if si: b.append('SI='+'/'.join(si))
        out.append('%s:%s%s'%(a,'+'.join(b) or 'NONE',(':'+fl) if fl else ''))
    print('#%d %s pdb=%s ids=%s\n   %s'%(n,t,pdb[:100],{k:v[:2] for k,v in ids.items()},' '.join(out)))
