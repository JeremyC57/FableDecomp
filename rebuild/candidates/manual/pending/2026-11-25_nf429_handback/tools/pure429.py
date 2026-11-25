#!/usr/bin/env python3
"""pure429.py <Class...>: for each new pure-virtual slot of <Class> (FableWin.pdb LF_ONEMETHOD `pure intro virtual`, vftable offset/4),
the override spelling in a derived class's retail vftable at that slot whose member name is the PDB name (two sources) ->
prints genmodel `--pure N=<mangled>` arguments (or MISSING)."""
import sys,os,re,collections,csv
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__))); import vtlib429 as V
T='/tmp/claude-0/egod_pdb/FableWin.types.txt'
R='/home/user/FableDecomp'
txt=open(T,errors='replace').read()
FL={}
for m in re.finditer(r'^\s*(0x[0-9A-F]+) \| LF_FIELDLIST \[size = \d+\]\n(.*?)(?=^\s*0x[0-9A-F]+ \| LF_)',txt,re.M|re.S): FL[m.group(1)]=m.group(2)
def fields(X):
    for m in re.finditer(r'LF_CLASS \[size = \d+\] `%s`\n.*?\n.*?field list: (0x[0-9A-F]+)'%re.escape(X),txt):
        if m.group(1) in FL: return FL[m.group(1)]
    return ''
D=collections.defaultdict(list)
for l in open(R+'/ghidra_out/rtti_bases.tsv'):
    p=l.rstrip('\n').split('\t')
    if len(p)>4 and '?A0x' not in p[1]:
        for b in p[4].split(';'):
            if b: D[b.split('@')[0]].append(p[1])
for X in sys.argv[1:]:
    f=fields(X); out=[]
    for m in re.finditer(r'LF_ONEMETHOD \[name = `(\w+)`\]\n\s*type = (0x[0-9A-F]+), vftable offset = (\d+), attrs = (\w+) pure intro virtual',f):
        nm,ty,off,acc=m.groups(); n=int(off)//4; hit=None
        for d in sorted(D[X]):
            for vt,sl in V.vtable(d):
                if len(sl)>n:
                    lab=V.L.get('%08x'%sl[n],'')
                    if lab.startswith('?%s@%s@@'%(nm,d)): hit=lab; break
            if hit: break
        orig=hit
        if hit:
            k=hit.index('@@')+2; L={'public':'U','protected':'M','private':'E'}[acc]
            if hit[k]!=L: hit=hit[:k]+L+hit[k+1:]   # the base's own access (PDB attrs), not the override's
        out.append('--pure %d=%s!%s'%(n,hit,orig) if hit else 'MISSING:%d:%s'%(n,nm))
    print(X,' '.join(out))
