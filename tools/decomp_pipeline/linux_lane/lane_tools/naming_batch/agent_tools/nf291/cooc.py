# cooc.py <class-token> <member>...: landed sources / tests whose CODE (comments stripped) names the class token AND, on any line, the
# member as a whole word; per hit, the code lines naming the member (to judge whether the member is reached through that class)
import os,re,sys
R='/home/user/FableDecomp/rebuild/'; cls=sys.argv[1]; mem=sys.argv[2:]
for d in ('src/compiled','tests'):
    for root,_,fs in os.walk(R+d):
        for f in fs:
            p=os.path.join(root,f)
            try: t=open(p,encoding='utf-8',errors='replace').read()
            except Exception: continue
            if cls not in t: continue
            code=[l.split('//')[0] for l in t.splitlines()]
            if not any(re.search(r'\b%s\b'%re.escape(cls),l) for l in code): continue
            for m in mem:
                ls=[l.strip()[:150] for l in code if re.search(r'\b%s\b'%re.escape(m),l)]
                if ls: print('%s\t%s\t%s\t%s'%(os.path.relpath(p,R),cls,m,' || '.join(ls[:3])))
