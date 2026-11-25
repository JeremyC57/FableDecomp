import json,collections,sys,re
R='/home/user/FableDecomp'; A=sys.argv[1]
t=json.load(open(A+'/evidence/occ_tu.json'))
cand=collections.defaultdict(list)
pat=re.compile(r'^(CDefPointer|CIVCountedPointer)<.*>::(~|Release$)')
for l in open(R+'/ghidra_out/fablewin_obj_functions.tsv',errors='replace'):
    f=l.rstrip('\n').split('\t')
    if len(f)>=7 and pat.match(f[5]): cand[f[0]].append((int(f[1]),f[6],int(f[3])))
ret=collections.defaultdict(list); diff=[]
for k,v in t.items():
    if v[5]=='same': ret[v[2]].append(k)
    else: diff.append(k)
eq=ne=0; rows=[]
for o in sorted(set(ret)|set(cand), key=lambda o:-len(ret.get(o,[]))):
    a=len(ret.get(o,[])); b=len(cand.get(o,[]))
    rows.append((o,a,b))
    if a and a==b: eq+=a
    elif a: ne+=a
print('occ in count-equal TUs',eq,'in mismatched TUs',ne,'boundary',len(diff))
for o,a,b in rows:
    if a: print(a,b,'EQ' if a==b else '--',o)
json.dump({o:sorted(v) for o,v in ret.items()},open(A+'/evidence/tu_occ.json','w'))
json.dump(cand,open(A+'/evidence/tu_cand.json','w'))
