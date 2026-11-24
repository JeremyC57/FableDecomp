# tblafter3.py: apply TABLE_CHANGES.tsv to reloc_bindings.tsv in memory; per touched target print its remaining spellings and whether the
# ENTRIES PDB spelling is among them (PDB_OK / NO_PDB). NF314.
import collections
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF314/'
T=collections.OrderedDict()
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: T[(r[0],r[1].lower())]=r[2]
P={l.split('\t')[0]:l.split('\t')[6] for l in open(A+'ENTRIES.tsv').read().splitlines()[1:]}
touched=[]
for l in open(A+'TABLE_CHANGES.tsv').read().splitlines()[1:]:
    a,s,t,w=l.split('\t')[:4]; touched.append(t)
    if a=='delete': T.pop((s,t),None)
    elif a=='add': T[(s,t)]=w
    elif a=='rewitness': T[(s,t)]=w
    elif a=='replace':
        o,n=[x.strip() for x in s.split('->')]; T.pop((o,t),None); T[(n,t)]=w
for t in dict.fromkeys(touched):
    sp=[s for (s,tt) in T if tt==t]
    print(t,'PDB_OK' if P.get(t) in sp else 'NO_PDB','|',' || '.join(sp))
