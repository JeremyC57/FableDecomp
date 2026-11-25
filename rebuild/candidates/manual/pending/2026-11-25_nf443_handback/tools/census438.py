#!/usr/bin/env python3
"""census438.py: for every T1_A target t (tracker/OPEN.tsv) and every table symbol s bound to t with s != PDB spelling, list the
landed rows (other than t's self row) whose source names s's identifier AND whose retail bytes reference t (E8/E9/imm scan).
-> work/census.tsv (row, target, wrong, pdb, ident). The AST step (ast438.py) then confirms which rows bind s."""
import sys, os, re, csv, collections, struct, glob, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
R='/home/user/FableDecomp'; csv.field_size_limit(10**9)
op=[r for r in csv.DictReader(open('tracker/OPEN.tsv'),delimiter='\t')]
tab=collections.defaultdict(set)
for r in csv.DictReader(open(R+'/rebuild/oracles/reloc_bindings.tsv'),delimiter='\t'): tab[r['target']].add(r['symbol'])
files={}
for f in glob.glob(R+'/rebuild/src/compiled/*/*/*.cpp'):
    m=re.search(r'_([0-9a-f]{8})\.cpp$',f)
    if m: files.setdefault(m.group(1),f)
tok=collections.defaultdict(set)
for a,f in files.items():
    for w in set(re.findall(r'\w+',open(f,encoding='utf-8',errors='replace').read())): tok[w].add(a)
cat=nf.catalog(); _c={}
def calls(a):
    if a in _c: return _c[a]
    out=set()
    if a in cat:
        b=bytes.fromhex(cat[a]['bytes']); va=int(a,16)
        for i in range(len(b)-4):
            if b[i] in (0xE8,0xE9): out.add('%08x'%((va+i+5+struct.unpack_from('<i',b,i+1)[0])&0xffffffff))
        for i in range(len(b)-3): out.add('%08x'%struct.unpack_from('<I',b,i)[0])
    _c[a]=out; return out
wr=sorted({s for r in op for s in tab[r['target']] if s!=r['pdb_spelling']})
dem={}
out=subprocess.run(['llvm-undname'],input='\n'.join(wr),capture_output=True,text=True).stdout
for b in out.strip().split('\n\n'):
    l=b.split('\n')
    if len(l)>=2: dem[l[0]]=l[1]
def ident(s):
    m=re.match(r'^\?\?[0-1]\??\$?(\w+)@',s)
    if m: return m.group(1)
    if s.startswith('??'): return 'operator'
    m=re.match(r'^\?(\w+)@',s)
    if m: return m.group(1)
    m=re.match(r'^[_@]?(\w+?)(@\d+)?$',s)
    return m.group(1) if m else None
rows=[]
for r in op:
    t=r['target']; P=r['pdb_spelling']
    for s in tab[t]:
        if s==P: continue
        n=ident(s)
        if not n: continue
        for a in sorted(tok.get(n,())):
            if a==t: continue
            if t in calls(a): rows.append((a,t,s,P,n))
with open('work/census.tsv','w') as f:
    for x in rows: f.write('\t'.join(x)+'\n')
print('lines',len(rows),'rows',len({x[0] for x in rows}),'targets',len({x[1] for x in rows}))
