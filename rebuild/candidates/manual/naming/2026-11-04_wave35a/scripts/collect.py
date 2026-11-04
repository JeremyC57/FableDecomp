# collect.py: union of the generated + graded rows (last family wins per address) -> $W/all_ok.tsv (fam, addr, json)
import sys,os,json,re,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from lib35 import *
FAMS=['ent','ent2','gesn','op','op2','op3','lop','sdd','def','sf1','opfmd']
rows={}
for f in FAMS:
    gp=W+'/'+f+'/gen.tsv'; pp=W+'/'+f+'/pass.tsv'
    if not os.path.exists(pp): continue
    ps={l.split('\t')[2]:l.split('\t')[0] for l in open(pp)}
    for l in open(gp):
        a=l.rstrip('\n').split('\t')
        if a[1]=='OK' and ps.get(a[0])=='PASS': rows[a[0]]=(f,json.loads(a[2]))
with open(W+'/all_ok.tsv','w') as o:
    for a,(f,d) in sorted(rows.items()): o.write('%s\t%s\t%s\n'%(f,a,json.dumps(d)))
print(len(rows),collections.Counter(f for f,d in rows.values()))
# duplicate-symbol checks
syms=collections.Counter(d['sym'] for f,d in rows.values()); print('dup syms in wave:',[s for s,c in syms.items() if c>1])
ent=ps1_entries(); mine=set(rows)
leafmap=collections.defaultdict(list)
for a,e in ent.items():
    if a in mine: continue
    try: s=open(ROOT+'/rebuild/src/compiled/'+e['Source'],errors='replace').read(4000)
    except: continue
    m=re.search(r'^//\s*Leaf:\s*(\S+)',s,re.M)
    if m: leafmap[m.group(1)].append(a)
clash=[(a,d['sym'],leafmap[d['sym']]) for a,(f,d) in rows.items() if d['sym'] in leafmap]
print('clash with other landed leaves:',clash)
cat=catalog()
cn=collections.Counter(q[1] for q in cat.values())
print('catalog name dup:',[(a,d['cat'],cn[d['cat']]) for a,(f,d) in rows.items() if cn[d['cat']]>1][:10])
