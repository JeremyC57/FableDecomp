# srcwide.py <Class> <Ident> [files...]: every landed source (or the given files) that declares Ident inside a local `class|struct Class`
# body, with the access label in effect and virtual / plain: finds binders that reach a target through a vtable slot or another path the
# exe call-site scan (xsites) cannot see. Prints: file  line  access  virtual|plain  (source reading only).
import re,sys,os
R='/home/user/FableDecomp/rebuild/src/compiled'
cls,ident=sys.argv[1],sys.argv[2]; files=sys.argv[3:]
if not files:
    for root,_,fs in os.walk(R):
        for f in fs:
            if f.endswith('.cpp'): files.append(os.path.join(root,f))
rx=re.compile(r'\b%s\s*\('%re.escape(ident)); hx=re.compile(r'\b(class|struct)\s+%s\b[^;]*$'%re.escape(cls))
def code(l): k=l.find('//'); return l if k<0 else l[:k]
for p in files:
    t=open(p,encoding='utf-8',errors='replace').read()
    if ident not in t or cls not in t: continue
    C=[code(x) for x in t.split('\n')]
    i=0
    while i<len(C):
        if hx.search(C[i]) and '{' in ''.join(C[i:i+2]):
            d=0; acc='private' if 'class' in hx.search(C[i]).group(1) else 'public'; j=i; started=False
            while j<len(C):
                c=C[j]
                seg=c[c.find('{')+1:] if (not started and '{' in c) else c
                for m in re.finditer(r'\b(public|protected|private)\s*:',seg if d<=1 else ''):
                    pass
                if d==1 or (not started and '{' in c):
                    pre=seg
                    if rx.search(pre) and re.search(r'\)\s*(const)?\s*(=\s*0)?\s*;',pre):
                        mm=list(re.finditer(r'\b(public|protected|private)\s*:',pre[:rx.search(pre).start()]))
                        a2=mm[-1].group(1) if mm else acc
                        print('%s\tL%d\t%s\t%s'%(p.rsplit('/',1)[-1],j+1,a2,'virtual' if 'virtual' in pre else 'plain'))
                    mm=list(re.finditer(r'\b(public|protected|private)\s*:',pre))
                    if mm: acc=mm[-1].group(1)
                d+=c.count('{')-c.count('}')
                if '{' in c: started=True
                if started and d<=0: break
                j+=1
            i=j
        i+=1
