# mdiff2.py <from> <to>: demangler-based (naming/msvc_demangle.py) stand-in v PDB classification per (entry, stand-in, witness), for the
# `??` / template names mdiff.py cannot split: LEAF (name differs), CLASS (class / scope base name differs), SCOPE (same class name, other
# namespace path), TAG (only struct/class tags differ, U v V), SIG (parameters / return differ beyond tags), QUAL (only access / const /
# virtual / storage letters differ), UNDEM (not demangled); plus DINK when the PDB spelling is a VC10 Dinkumware form (NF301 done-equivalent)
# and the off-limits reasons of target and witness (work/offwhy.tsv).
import re,sys,collections
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline/naming'); import msvc_demangle as MD
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF311'
why=collections.defaultdict(set)
for l in open(A+'/work/offwhy.tsv'):
    a,r=l.split(); why[a].add(r)
DINK=re.compile(r'_Vector_val|_List_val|_Tree_val|_String_val|_Container_base|_Iterator_base|_Vector_const_iterator|_List_const_iterator|_Tree_const_iterator|_Tree_nod|_List_nod|_Tmap_traits|_Tset_traits|_Ranit|_Bidit|U\?\$char_traits')
def dm(s):
    try: d=MD.demangle(s)
    except Exception: return None
    return d if d.get('ok') else None
untag=lambda x: re.sub(r'\b(class|struct|union|enum) ','',x)
def cmp(p,s):
    a,b=dm(p),dm(s)
    if not a or not b: return 'UNDEM'
    if a.get('name')!=b.get('name'): return 'LEAF'
    ca,cb=a['comps'][:-1],b['comps'][:-1]
    pa,pb=(a.get('params') or [],a.get('ret') or ''),(b.get('params') or [],b.get('ret') or '')
    if ca!=cb:
        if [untag(x) for x in ca]==[untag(x) for x in cb]: v='TAG'
        elif ca and cb and ca[-1].split('<')[0]==cb[-1].split('<')[0]: v='SCOPE' if ca[-1]==cb[-1] else 'SCOPE~'
        elif not ca or not cb: v='FREE'
        else: v='CLASS'
    else: v=''
    if pa!=pb:
        v+=('+' if v else '')+('TAG' if ([untag(x) for x in pa[0]],untag(pa[1]))==([untag(x) for x in pb[0]],untag(pb[1])) else 'SIG')
    if not v: v='QUAL'
    return v
L=open(A+'/ENTRIES.tsv').read().splitlines()
for n in range(int(sys.argv[1]),int(sys.argv[2])+1):
    c=L[n].split('\t'); t,pdb,sp=c[0],c[6],c[11]
    dk='DINK' if DINK.search(pdb) else '-'
    for part in sp.split(' || '):
        m=re.match(r'(\S+) \(([^)]*)\)',part)
        if not m or m.group(1)==pdb: continue
        sym,wit=m.group(1),m.group(2); v=cmp(pdb,sym)
        for w in wit.split(','):
            print('#%d\t%s\t%s\t%s\t%s\tT:%s\tW:%s\t%s'%(n,t,w,v,dk,','.join(sorted(why[t])) or '-',','.join(sorted(why[w])) or '-',sym[:120]))
