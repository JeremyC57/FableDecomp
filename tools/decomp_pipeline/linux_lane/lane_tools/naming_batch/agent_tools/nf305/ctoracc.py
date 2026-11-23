# ctoracc.py <Class>: every landed source/test/header that declares `Class();` inside a local `class|struct Class` body, with the access
# label in force at that line (struct default public, class default private). Census for a ctor-access stand-in (??0X@@QAE v IAE).
import os,re,sys
R='/home/user/FableDecomp/rebuild/'; C=sys.argv[1]
head=re.compile(r'^\s*(class|struct)\s+%s\b[^;]*$'%C)
for d in ('src/compiled','tests','include'):
    for root,_,fs in os.walk(R+d):
        for f in fs:
            p=os.path.join(root,f)
            try: t=open(p,encoding='utf-8',errors='replace').read()
            except Exception: continue
            if C+'()' not in t: continue
            L=[l.split('//')[0] for l in t.splitlines()]
            i=0
            while i<len(L):
                m=head.match(L[i])
                if m and '{' in ''.join(L[i:i+2]):
                    acc='public' if m.group(1)=='struct' else 'private'; depth=0; j=i
                    while j<len(L):
                        for mm in re.finditer(r'\b(public|protected|private)\s*:',L[j]):
                            if depth<=1: acc=mm.group(1)
                        if re.search(r'(^|[^~\w])%s\s*\(\s*(void)?\s*\)\s*[;{]'%C,L[j]) and depth<=1 and j>i or (j==i and re.search(r'[{;]\s*%s\s*\(\s*\)\s*;'%C,L[j])):
                            print('%s\t%d\t%s'%(acc,j+1,os.path.relpath(p,R)))
                        depth+=L[j].count('{')-L[j].count('}')
                        if depth<=0 and '{' in ''.join(L[i:j+1]): break
                        j+=1
                    i=j
                i+=1
