# vdecl.py <entry#> <Class> <Ident>: for every landed exe caller row of the entry's target (work/xs_all.tsv) plus its table witnesses,
# list each DECLARATION of Ident inside a local `class|struct Class` body (access label in effect, `virtual` or not, the class head),
# and whether the row includes a shared engine/<Class>.h. Source reading only (a census aid; what an object binds is read from objects).
import re,sys,subprocess
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF303'
R='/home/user/FableDecomp/rebuild/'
ps=open(R+'build_candidates.ps1',encoding='utf-8').read()
so=set(open(A+'/work/stayoff.txt').read().split()); t3=set(open(A+'/work/t3rows.txt').read().split())
def src(a):
    i=ps.find("Address = '%s'"%a)
    if i<0: return None
    m=re.search(r"\bSource = '([^']+)'",ps[i:ps.find("\n    }",i)]); return R+'src/compiled/'+m.group(1) if m else None
def code(l): k=l.find('//'); return l if k<0 else l[:k]
L=[l.split('\t') for l in open(A+'/ENTRIES.tsv').read().splitlines()]
n=int(sys.argv[1]); cls=sys.argv[2]; ident=sys.argv[3]; t=L[n][0]
rows=set()
for l in open(A+'/work/xs_all.tsv'):
    p=l.rstrip('\n').split('\t')
    if p[0]==t and p[3]!='-' and p[3][8]=='L': rows.add(p[3][:8])
for w in re.findall(r'\(([0-9a-f]{8})(?:,self)?\)',L[n][11]): rows.add(w)
rx=re.compile(r'\b%s\s*\('%re.escape(ident))
for a in sorted(rows):
    f=src(a)
    if not f: print(a,'NOSRC'); continue
    T=open(f,encoding='utf-8',errors='replace').read().split('\n'); C=[code(x) for x in T]
    inc='INC' if re.search(r'#include\s+"engine/%s\.h"'%re.escape(cls),'\n'.join(C)) else '-'
    fl=('SO ' if a in so else '')+('T3 ' if a in t3 else '')
    out=[]
    for i,c in enumerate(C):
        if not rx.search(c) or not re.search(r'\)\s*(const)?\s*(=\s*0)?\s*;',c): continue
        if re.search(r'(::|\.|->)\s*%s\s*\('%re.escape(ident),c): continue
        # find enclosing class head and access
        acc=None; head=None; depth=0
        pre=c[:rx.search(c).start()]; m=list(re.finditer(r'\b(public|protected|private)\s*:',pre))
        if m: acc=m[-1].group(1)
        j=i-1
        while j>=0 and head is None:
            cj=C[j]; depth+=cj.count('}')-cj.count('{')
            if depth<0:
                mh=re.search(r'\b(class|struct)\s+(\w+)',cj); head=(mh.group(1),mh.group(2)) if mh else ('?','?')
                if acc is None:
                    mm=list(re.finditer(r'\b(public|protected|private)\s*:',cj[cj.find('{'):] if '{' in cj else ''))
                    acc=mm[-1].group(1) if mm else ('private' if head[0]=='class' else 'public')
                break
            if acc is None and depth==0:
                mm=list(re.finditer(r'\b(public|protected|private)\s*:',cj))
                if mm: acc=mm[-1].group(1)
            j-=1
        if head is None: head=('file','-')
        # NF303: enclosing namespace(s) of the class head (brace walk from the head line upward)
        ns=[]; d=0; k=j-1
        while k>=0:
            ck=C[k]; d+=ck.count('}')-ck.count('{')
            if d<0:
                mn=re.search(r'\bnamespace\s+(\w+)',ck)
                if mn: ns.insert(0,mn.group(1))
                d=0
            k-=1
        head=(head[0],'::'.join(ns+[head[1]]))
        v='virtual' if re.search(r'\bvirtual\b',c) else 'plain'
        out.append('L%d:%s:%s:%s:%s'%(i+1,head[1],acc,v,c.strip()[:70].replace('\t',' ')))
    print('%s %s%s %s | %s'%(a,fl,inc,f.rsplit('/',1)[-1][:60],' || '.join(out) if out else 'NODECL'))
