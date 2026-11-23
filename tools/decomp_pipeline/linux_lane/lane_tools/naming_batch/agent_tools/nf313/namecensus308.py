# namecensus308.py: for each new name an NF313 row now binds or defines, list landed sources / tests that DEFINE it (a line with the
# name followed by `(` and a `{` body on the same or next line, not a call); owner rows are expected, anything else is read by hand.
import os,re
R='/home/user/FableDecomp/rebuild/'
N=['SetRotationAngle','PerformSequenceReset','SetLungedThisFrame','GetLineThingSearchArea','GFAnchorToTopRight','StopProgress']
hits={n:[] for n in N}
for d in ('src/compiled','tests'):
    for root,_,fs in os.walk(R+d):
        for f in fs:
            if not f.endswith('.cpp'): continue
            p=os.path.join(root,f)
            try: L=open(p,encoding='utf-8',errors='replace').read().splitlines()
            except Exception: continue
            for i,l in enumerate(L):
                if l.lstrip().startswith('//'): continue
                for n in N:
                    m=re.search(r'\b(?:\w+::)*%s\s*\('%n,l)
                    if not m: continue
                    pre=l[:m.start()]   # NF313 fix: filter only the text BEFORE the name (a one-line `{return ...}` body is a definition)
                    if not re.fullmatch(r'[\w:<>\*&\s]*',pre) or re.search(r'\b(return|if|while|else)\b',pre) or re.search(r'[=.>(,]\s*$',pre): continue
                    if ('{' in l[m.end():] or (i+1<len(L) and L[i+1].strip().startswith('{'))) and ';' not in l[m.end():].split('{')[0]:
                        hits[n].append('%s:%d: %s'%(p.replace(R,''),i+1,l.strip()[:120]))
for n in N:
    print('##',n,len(hits[n]))
    for h in hits[n]: print('  ',h)
