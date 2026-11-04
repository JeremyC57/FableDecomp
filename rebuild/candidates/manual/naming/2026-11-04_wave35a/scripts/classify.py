import sys,os,re,json,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from common import *
W=os.environ['S']+'/wave35a'
ent=ps1_entries(); cat=catalog()
names=collections.defaultdict(set)  # sym -> pdbs
for fn,tag in (('egor_pdb_names.tsv','E'),('fablewin_pdb_names.tsv','F')):
    for l in open(ROOT+'/ghidra_out/'+fn):
        if '\t' in l: names[l.rstrip('\n').split('\t',1)[1]].add(tag)
# index by prefix "?M@E@O@NScript@@"
byq=collections.defaultdict(list)
for s in names:
    m=re.match(r'^(\?\?_[GE1]|\?[A-Za-z_0-9]+@)([A-Za-z_0-9@]+?@NScript@@)',s)
    if m: byq[m.group(1)+m.group(2)].append(s)
allocs={}
for a,q in cat.items():
    if q[1].endswith('::Alloc'): allocs.setdefault(q[1],[]).append(a)
rows=[l.rstrip('\n').split('\t') for l in open(W+'/ROWS.tsv')][1:]
out=[]
for addr,cn,leaf,src in rows:
    m=re.match(r"^NScript::(\w+)::(\w+)::(\w+|`scalar deleting destructor')$",cn)
    rec=dict(addr=addr,cn=cn,leaf=leaf,src=src,len=cat[addr][2])
    if m:
        O,E,M=m.groups(); rec.update(kind='ent',O=O,E=E,M=M)
        key=('??_G' if M.startswith('`') else '?'+M+'@')+E+'@'+O+'@NScript@@'
        rec['pdb']=sorted(byq.get(key,[])); 
        if M.startswith('`'): rec['pdbE']=sorted(byq.get('??_E'+E+'@'+O+'@NScript@@',[]))
        al=allocs.get('NScript::%s::%s::Alloc'%(O,E),[])
        rec['alloc']=[(x, x in ent) for x in al]
        rec['hdr']=[h for h in ('engine/%s.h'%O,'engine/NScript_%s.h'%O) if os.path.exists(ROOT+'/rebuild/include/'+h)]
    else:
        rec['kind']='other'
    out.append(rec)
json.dump(out,open(W+'/classify.json','w'),indent=0)
c=collections.Counter()
for r in out:
    if r['kind']!='ent': c['other']+=1; continue
    c[(r['M'][:12],'pdb%d'%len(r['pdb']),'alloc'+''.join('L' if x[1] else 'n' for x in r['alloc']),'hdr' if r['hdr'] else '-')]+=1
for k,v in sorted(c.items(),key=lambda x:-x[1]): print(v,k)
