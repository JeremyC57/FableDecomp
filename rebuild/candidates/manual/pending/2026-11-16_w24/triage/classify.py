#!/usr/bin/env python3
"""classify.py <outdir> <oracle.tsv> -> TSV: addr, status, retail_len, built_len, difflines, class, len_src"""
import sys,os,re,collections,csv
csv.field_size_limit(10**8)
d=sys.argv[1]; lsrc={r['address']:r['len_src'] for r in csv.DictReader(open(sys.argv[2]),delimiter='\t')}
def norm(ins):
    ins=re.sub(r'\[(0x)?[0-9a-f]+\]','[A]',ins); ins=re.sub(r'0x[0-9a-f]{6,}','A',ins); return ins
print('addr\tstatus\tretail\tbuilt\tdifflines\tclass\tlen_src')
for f in sorted(os.listdir(d)):
    a=f[:8]; t=open(os.path.join(d,f),errors='replace').read(); st=(t.splitlines() or [''])[0]
    m=re.search(r' (MATCH|RELOCATION_MATCH|DIFFER\((\d+)v(\d+)\)|SRC_FAIL|\w+FAIL\w*)',st)
    if not m: print(a,'NO_RESULT','','','','',lsrc.get(a,''),sep='\t'); continue
    s=m.group(1)
    if not s.startswith('DIFFER') or '--- built' not in t:
        print(a,s.split('(')[0],'','','','',lsrc.get(a,''),sep='\t'); continue
    built,ret=int(m.group(2)),int(m.group(3))
    A=[l.split(None,2)[2] for l in t.split('--- retail')[1].split('--- built')[0].splitlines()[1:] if len(l.split(None,2))==3]
    B=[l.split(None,2)[2] for l in t.split('--- built')[1].splitlines()[1:] if len(l.split(None,2))==3]
    def nn(x): 
        x=norm(x)
        if x.startswith(('call','jmp','j')): x=re.sub(r'0x[0-9a-f]+','T',x)
        return x
    NA=[nn(x) for x in A]; NB=[nn(x) for x in B]
    n=sum(1 for x,y in zip(NA,NB) if x!=y)+abs(len(NA)-len(NB))
    if n==0: cls='masked_equal'
    elif built==ret and collections.Counter(NA)==collections.Counter(NB): cls='tie'
    elif built==ret: cls='same_size'
    else: cls='size%+d'%(built-ret)
    print(a,'DIFFER',ret,built,n,cls,lsrc.get(a,''),sep='\t')
