import json,sys,collections
sys.path.insert(0,sys.argv[1]+'/tools')
from pe import PE
A=sys.argv[1]; obj=sys.argv[2]
R='/home/user/FableDecomp'
r=json.load(open(A+'/status/rec.json')); occ=json.load(open(A+'/evidence/occ.json'))
tu=json.load(open(A+'/evidence/tu_occ.json')); c=json.load(open(A+'/evidence/tu_cand.json'))
cat={}
for l in open(R+'/rebuild/oracles/auto-re-candidates.tsv'):
    f=l.split('\t')
    if len(f)>2 and f[2].isdigit(): cat[int(f[0],16)]=(f[1],int(f[2]))
dbg=[]
for l in open(R+'/ghidra_out/fablewin_obj_functions.tsv',errors='replace'):
    f=l.rstrip('\n').split('\t')
    if f[0]==obj: dbg.append((int(f[1]),f[5],int(f[3])))
dbg.sort()
idx=[i for i,nm,s in c[obj]]
W=int(sys.argv[3]) if len(sys.argv)>3 else 3
print('--- debug')
shown=set()
for i in idx:
    for j in range(i-W,i+W+1):
        if j in shown: continue
        shown.add(j)
        e=[x for x in dbg if x[0]==j]
        if e: print(('*' if j in idx else ' '),j,e[0][2],e[0][1][:110])
    print('   ..')
print('--- retail')
ks=sorted(cat)
import bisect
for a in tu[obj]:
    ai=int(a,16); p=bisect.bisect_left(ks,ai)
    for q in ks[max(0,p-W):p+W+1]:
        k='%08x'%q; e=r.get(k,{})
        nm=(e.get('self_leaf') or e.get('pdb_spelling') or cat[q][0]) if e else cat[q][0]
        print(('*' if k in occ else ' '),k,cat[q][1],nm[:110])
    print('   ..')
