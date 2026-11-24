# tbl.py: per ENTRIES line, the reloc_bindings.tsv lines for its target, marked PDB (= the entry's pdb_spelling) or SI (stand-in), with witness
import sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF314'
T={}
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: T.setdefault(r[1].lower(),[]).append((r[0],r[2]))
L=[l.split('\t') for l in open(A+'/ENTRIES.tsv').read().splitlines()]
for n,c in enumerate(L[1:],1):
    print('#%d %s self=%s'%(n,c[0],'Y' if c[7] else '-'))
    for s,w in T.get(c[0].lower(),[]): print('   %s %s %s'%('PDB' if s==c[6] else 'SI ',w,s[:150]))
