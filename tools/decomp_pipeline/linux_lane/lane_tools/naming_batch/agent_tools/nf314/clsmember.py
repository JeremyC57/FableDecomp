# clsmember.py <class> <member> [root]: every landed source (rebuild/src/compiled, comments stripped) and every rebuild/include header that
# declares <member>( inside a `class|struct <class>` block: prints file, access label in force, virtual or not, const or not.
# Used for the vftable-emitter side of a delete audit (a row that emits / calls a slot binds the base spelling). NF314.
import sys,os,re
cls,mem=sys.argv[1],sys.argv[2]; roots=sys.argv[3:] or ['/home/user/FableDecomp/rebuild/src/compiled','/home/user/FableDecomp/rebuild/include']
head=re.compile(r'\b(class|struct)\s+%s\b[^;{(]*\{'%re.escape(cls))
for root in roots:
    for d,_,fs in os.walk(root):
        for f in fs:
            if not f.endswith(('.cpp','.h')): continue
            p=os.path.join(d,f); t=open(p,encoding='utf-8',errors='replace').read()
            if cls not in t or mem not in t: continue
            t=re.sub(r'//[^\n]*','',t)
            for m in head.finditer(t):
                i=m.end(); depth=1; j=i
                while j<len(t) and depth:
                    if t[j]=='{': depth+=1
                    elif t[j]=='}': depth-=1
                    j+=1
                body=t[i:j]; acc='private' if m.group(1)=='class' else 'public'
                for k,line in enumerate(re.split(r'(?<=[;{}])',body)):
                    for a in re.findall(r'\b(public|protected|private)\s*:',line): acc=a
                    if re.search(r'\b%s\s*\('%re.escape(mem),line) and not re.search(r'::\s*%s'%re.escape(mem),line):
                        print('%s\t%s\t%s\t%s'%(os.path.relpath(p,'/home/user/FableDecomp/rebuild'),acc,'virtual' if 'virtual' in line else 'nonvirtual',' '.join(line.split())[:120]))
