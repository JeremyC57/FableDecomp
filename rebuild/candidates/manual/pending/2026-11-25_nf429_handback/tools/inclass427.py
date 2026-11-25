#!/usr/bin/env python3
"""inclass427.py: for every ACCEPT row's new self C::M, search landed sources and headers (rebuild/include + hdr/) that mention C for an
in-class definition `M(...) [const] {` inside a `class|struct C {...}` body (a pick-any definer of the same symbol), and any
reloc_bindings / self_symbols line binding the new self at another address. -> evidence/inclass_census.tsv"""
import glob,re,os,csv,collections
R='/home/user/FableDecomp'; A=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sel=[]
for B in sorted(glob.glob(A+'/batches/*')):
    for r in csv.DictReader(open(B+'/RESULTS.tsv'),delimiter='\t'):
        m=re.match(r'\?(\w+)@(\w+)@@',r['new']); sel.append((os.path.basename(B),r['address'],m.group(2),m.group(1),r['new']))
classes=set(c for _,_,c,_,_ in sel)
files=glob.glob(R+'/rebuild/src/compiled/**/*.cpp',recursive=True)+glob.glob(R+'/rebuild/include/**/*.h',recursive=True)+glob.glob(A+'/hdr/engine/*.h')
pat=re.compile(r'\b('+'|'.join(sorted(classes,key=len,reverse=True))+r')\b')
idx=collections.defaultdict(list)
for p in files:
    t=open(p,errors='replace').read()
    for c in set(pat.findall(t)): idx[c].append((p,re.sub(r'//[^\n]*','',t)))
def bodies(t,C):
    for m in re.finditer(r'\b(class|struct)\s+%s\b[^;{]*\{'%re.escape(C),t):
        i=m.end(); d=1
        while d and i<len(t):
            if t[i]=='{': d+=1
            elif t[i]=='}': d-=1
            i+=1
        yield t[m.end():i]
out=[]
for b,a,C,M,new in sel:
    for p,t in idx[C]:
        for body in bodies(t,C):
            if re.search(r'\b%s\s*\([^;{}]*\)\s*(const\s*)?\{'%re.escape(M),body): out.append(('INCLASS',b,a,new,os.path.relpath(p,R)))
tab=collections.defaultdict(set)
for r in csv.DictReader(open(R+'/rebuild/oracles/reloc_bindings.tsv'),delimiter='\t'): tab[r['symbol']].add(r['target'].lower())
ss=collections.defaultdict(set)
for l in open(R+'/rebuild/oracles/self_symbols.tsv'):
    f=l.rstrip('\n').split('\t')
    if len(f)>1: ss[f[1]].add(f[0].lower().replace('0x',''))
for b,a,C,M,new in sel:
    o=(tab.get(new,set())|ss.get(new,set()))-{a}
    if o: out.append(('BOUND_ELSEWHERE',b,a,new,','.join(sorted(o))))
with open(A+'/evidence/inclass_census.tsv','w') as f:
    f.write('kind\tbatch\taddress\tnew_self\twhere\n')
    for x in out: f.write('\t'.join(x)+'\n')
print(len(sel),'selves;',collections.Counter(x[0] for x in out))
for x in out: print('\t'.join(x)[:250])
