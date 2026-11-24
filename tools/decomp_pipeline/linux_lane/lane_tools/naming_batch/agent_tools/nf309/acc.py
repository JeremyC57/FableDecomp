# acc.py <file> <regex-of-declaration-line>: for each matching declaration line, print the access label in effect (the nearest earlier
# `public:` / `protected:` / `private:` on or before the line inside the same class, or the class-key default) and the enclosing class head.
import re,sys
L=open(sys.argv[1],encoding='utf-8',errors='replace').read().split('\n'); rx=re.compile(sys.argv[2])
def code(l): k=l.find('//'); return l if k<0 else l[:k]
for i,l in enumerate(L):
    c=code(l)
    if not rx.search(c): continue
    acc=None; head=None; depth=0
    pre=c[:rx.search(c).start()]
    m=list(re.finditer(r'\b(public|protected|private)\s*:',pre))
    if m: acc=m[-1].group(1)
    j=i-1
    while j>=0 and head is None:
        cj=code(L[j]); depth+=cj.count('}')-cj.count('{')
        if depth<0:
            mh=re.search(r'\b(class|struct)\s+(\w+)',cj)
            head=(mh.group(1),mh.group(2)) if mh else ('?',cj.strip()[:40])
            if acc is None:
                mm=list(re.finditer(r'\b(public|protected|private)\s*:',cj[cj.find('{'):] if '{' in cj else ''))
                acc=mm[-1].group(1) if mm else ('private' if head[0]=='class' else 'public')
            break
        if acc is None and depth==0:
            mm=list(re.finditer(r'\b(public|protected|private)\s*:',cj))
            if mm: acc=mm[-1].group(1)
        j-=1
    print('%d\t%s\t%s\t%s'%(i+1,acc,head,c.strip()[:80]))
