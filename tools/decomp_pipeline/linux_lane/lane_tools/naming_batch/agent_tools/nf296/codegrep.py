# codegrep.py <identifier>...: landed sources and tests whose CODE (// comments stripped) names the identifier as a whole word
import os,re,sys
R='/home/user/FableDecomp/rebuild/'
ids=[(i,re.compile(r'\b%s\b'%re.escape(i))) for i in sys.argv[1:]]; hits={i:[] for i,_ in ids}
for d in ('src/compiled','tests'):
    for root,_,fs in os.walk(R+d):
        for f in fs:
            p=os.path.join(root,f)
            try: t=open(p,encoding='utf-8',errors='replace').read()
            except Exception: continue
            if not any(i in t for i,_ in ids): continue
            code='\n'.join(l.split('//')[0] for l in t.splitlines())
            for i,rx in ids:
                if rx.search(code): hits[i].append(os.path.relpath(p,R))
for i,_ in ids: print('%s\t%d\t%s'%(i,len(hits[i]),' '.join(hits[i][:8])))
