import sys, re, struct, collections
sys.path.insert(0, sys.argv[0].rsplit('/',1)[0])
from pe import PE
p=PE()
rows=[]
for l in open('rebuild/oracles/auto-re-candidates.tsv'):
    a=l.rstrip('\n').split('\t')
    if len(a)<4: continue
    if re.match(r"^CVtable_[0-9a-fA-F]+::`scalar deleting destructor'$",a[1]) or re.match(r'^_global::ScalarDeletingDtor_[0-9a-f]+$',a[1]):
        rows.append(a)
addrs={int(a[0],16) for a in rows}
# raw refs from .rdata/.data
refs=collections.defaultdict(list)
for s in p.secs:
    if s[0] not in ('.rdata','.data'): continue
    o=s[3]; n=s[4]
    for i in range(0,n-3,4):
        v=struct.unpack_from('<I',p.d,o+i)[0]
        if v in addrs: refs[v].append(s[1]+i)
vt={}
for l in open(sys.argv[1]):
    a=l.rstrip('\n').split('\t')
    if a[0]=='vtable': continue
    vt[(int(a[0],16)+4*int(a[3]))]=(a[0],a[1],a[2],a[3])
out=open(sys.argv[2],'w')
out.write('address\tname\tlength\tnrefs\trefs\trtti\n')
for a in rows:
    ad=int(a[0],16)
    r=refs.get(ad,[])
    rt=[vt[x] for x in r if x in vt]
    out.write('%s\t%s\t%s\t%d\t%s\t%s\n'%(a[0],a[1],a[2],len(r),','.join('%08x'%x for x in r),';'.join('%s/%s/%s/slot%s'%t for t in rt)))
