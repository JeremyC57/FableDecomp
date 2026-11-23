# tblafter.py: apply TABLE_CHANGES.tsv to reloc_bindings.tsv in memory and print, per STATUS entry, the spellings left for its target
# that differ from the ENTRIES pdb_spelling (a `done` entry must have none).
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF299/'
T={}
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: T[(r[0],r[1].lower())]=r[2]
for l in open(A+'TABLE_CHANGES.tsv').read().splitlines()[1:]:
    f=l.split('\t')
    if len(f)<4: continue
    k=(f[1],f[2].lower().replace('0x',''))
    if f[0]=='delete': T.pop(k,None)
    elif f[0]=='add': T[k]=f[3].replace('0x','')
    elif f[0]=='rewitness': T[k]=f[3].split('->')[-1].strip().replace('0x','')
E={l.split('\t')[0]:l.split('\t')[6] for l in open(A+'ENTRIES.tsv').read().splitlines()[1:]}
for l in open(A+'STATUS.tsv').read().splitlines()[1:]:
    f=l.split('\t'); t=f[0].replace('0x','')
    if t not in E: continue   # NF299: a follow-up line outside ENTRIES
    rest=[(s,w) for (s,tt),w in T.items() if tt==t and s!=E[t]]
    if rest: print(f[1],f[0],'; '.join('%s (%s)'%(s[:80],w) for s,w in rest))
