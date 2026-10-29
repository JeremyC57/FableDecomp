import csv,collections
ROOT='/home/user/FableDecomp'
def names(p):
    s=set()
    for l in open(p,encoding='utf-8',errors='replace'):
        q=l.rstrip('\n').split('\t')
        if len(q)>=2: s.add(q[1])
    return s
EG=names(ROOT+'/ghidra_out/egor_pdb_names.tsv'); FW=names(ROOT+'/ghidra_out/fablewin_pdb_names.tsv')
bound=collections.defaultdict(set); sym2=collections.defaultdict(set)
for r in csv.reader(open(ROOT+'/rebuild/oracles/reloc_bindings.tsv'),delimiter='\t'):
    if len(r)>=3 and r[0]!='symbol': bound[r[1]].add(r[0]); sym2[r[0]].add(r[1])
c=collections.Counter()
for l in open('QUEUE_566_574.tsv'):
    q=l.rstrip('\n').split('\t')
    a=q[1].lower().replace('0x','').zfill(8); s=q[2]; s2=s.replace('_W','G')
    src='+'.join(n for n,S in (('E',EG),('F',FW)) if s in S or s2 in S) or '-'
    wrong=sorted(bound.get(a,set())-{s2})
    sb=sorted(sym2.get(s2,set()))
    c[src]+=1
    print(q[0],a,src,'bound_other=%d'%len(wrong),'final_bound_to=%s'%','.join(sb),s[:110])
print(c)
