import json,collections,sys
R='/home/user/FableDecomp'
A=sys.argv[1]
r=json.load(open(A+'/status/rec.json'))
occ=json.load(open(A+'/evidence/occ.json'))
fw=collections.defaultdict(list); objfuncs=collections.defaultdict(list)
for l in open(R+'/ghidra_out/fablewin_obj_functions.tsv',errors='replace'):
    if l.startswith('#'): continue
    f=l.rstrip('\n').split('\t')
    if len(f)<7: continue
    objfuncs[f[0]].append((int(f[1]),f[5],f[6],int(f[3])))
    if f[6]: fw[f[6]].append(f[0])
anch={}
for k,e in r.items():
    if e['status'] in('agree_pdb',) and e['pdb_spelling'] and len(set(fw.get(e['pdb_spelling'],[])))==1:
        anch[int(k,16)]=fw[e['pdb_spelling']][0]
json.dump({'%08x'%a:o for a,o in anch.items()},open(A+'/evidence/anchors.json','w'))
print(len(anch))
# object order in debug
objorder={}
for l in open(R+'/ghidra_out/fablewin_obj_functions.tsv',errors='replace'):
    f=l.split('\t')
    if len(f)>1 and f[0] not in objorder: objorder[f[0]]=len(objorder)
ks=sorted(anch); prev=None; seq=[]
for a in ks:
    o=anch[a]
    if o!=prev: seq.append((a,o)); prev=o
print('segments',len(seq))
inv=0
for i in range(1,len(seq)):
    if objorder[seq[i][1]]<objorder[seq[i-1][1]]: inv+=1
print('order inversions',inv)
for a,o in seq[:40]: print('%08x'%a,objorder[o],o)
