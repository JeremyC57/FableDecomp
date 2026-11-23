# memacc.py <Class> <Member> [files...]: every landed source / test / header (or the given files) that declares Member inside a
# `class|struct Class` body: access label in force, const-ness, and the declaration text. Census for access / const stand-ins.
import os,re,sys
R='/home/user/FableDecomp/rebuild/'; C,M=sys.argv[1],sys.argv[2]
head=re.compile(r'^\s*(?:template\s*<[^>]*>\s*)?(class|struct)\s+%s\b(?!\s*;)'%re.escape(C))
def files():
    if len(sys.argv)>3:
        for f in sys.argv[3:]: yield f
        return
    for d in ('src/compiled','tests','include'):
        for root,_,fs in os.walk(R+d):
            for f in fs: yield os.path.join(root,f)
for p in files():
    try: t=open(p,encoding='utf-8',errors='replace').read()
    except Exception: continue
    if M not in t or C not in t: continue
    L=[l.split('//')[0] for l in t.splitlines()]
    i=0
    while i<len(L):
        m=head.match(L[i])
        if m and '{' in ''.join(L[i:i+3]):
            acc='public' if m.group(1)=='struct' else 'private'; depth=0; j=i; opened=False
            while j<len(L):
                line=L[j]
                # walk tokens in order so labels and decls on one line keep their order
                for tok in re.finditer(r'\b(public|protected|private)\s*:|\b%s\s*\(|[{}]'%re.escape(M),line):
                    s=tok.group(0)
                    if s=='{': depth+=1; opened=True
                    elif s=='}': depth-=1
                    elif tok.group(1):
                        if depth==1: acc=tok.group(1)
                    elif depth==1:
                        rest=line[tok.end():]; cst='const' if re.match(r'[^)]*\)\s*const\b',rest) else 'nonconst'
                        print('%s\t%s\t%d\t%s\t%s'%(acc,cst,j+1,os.path.relpath(p,R),line.strip()[:110]))
                if opened and depth<=0: break
                j+=1
            i=j
        i+=1
