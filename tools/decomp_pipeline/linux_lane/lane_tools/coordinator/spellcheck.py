# spellcheck.py <rows.tsv>: addresses bound/defined by the chunk's rows that carry another spelling elsewhere (bindings or landed Leaf)
# (2026-11-03) Moved from the session scratchpad into the repo; run from the repo root.
import sys,re,glob,csv,subprocess
chunk={r['address'].lower() for r in csv.DictReader(open(sys.argv[1]),delimiter='\t')}
b=[l.rstrip('\n').split('\t')[:3] for l in open('rebuild/oracles/reloc_bindings.tsv')][1:]
b=[x for x in b if len(x)==3]
new={tuple(l[1:].split('\t')[:3]) for l in subprocess.run(['git','diff','HEAD','rebuild/oracles/reloc_bindings.tsv'],capture_output=True,text=True).stdout.splitlines() if l.startswith('+') and not l.startswith('+++')}
bya={}
for s,t,c in b: bya.setdefault(t.lower(),{}).setdefault(s,set()).add(c.lower())
def leafof(a):
    out=[]
    for sp in glob.glob(f'rebuild/src/compiled/*/*/*_{a}.cpp'):
        for line in open(sp,errors='replace'):
            m=re.match(r'//\s*Leaf:\s*(\S+)',line)
            if m: out.append((sp.split('/')[-1],m.group(1))); break
    return out
used=set((s,t.lower()) for s,t,c in b if c.lower() in chunk)
for s,t in sorted(used,key=lambda x:x[1]):
    others={o:c for o,c in bya.get(t,{}).items() if o!=s}
    bad=[(p,l) for p,l in leafof(t) if not s.startswith(l)]
    isnew=any(x[0]==s and x[1].lower()==t for x in new)
    if others or bad: print(('NEW ' if isnew else 'old ')+t, s[:90], '\n    others:',{o[:80]:sorted(c)[:4] for o,c in others.items()}, '\n    landed leaf:',bad)
for a in sorted(chunk):
    for p,l in leafof(a):
        o={x:sorted(c)[:4] for x,c in bya.get(a,{}).items() if not x.startswith(l)}
        if o: print('LEAF',a,l[:70],'<- other spellings bound by',o)
