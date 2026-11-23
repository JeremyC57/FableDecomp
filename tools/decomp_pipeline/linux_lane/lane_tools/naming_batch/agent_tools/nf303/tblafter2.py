# tblafter2.py: NF303 version of tblafter.py that also applies `replace old -> new` lines; prints per `done*` STATUS entry whether the PDB
# spelling keeps a table line after TABLE_CHANGES and which other spellings remain on the target.
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF303/'
T={}
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: T[(r[0],r[1].lower())]=r[2]
for l in open(A+'TABLE_CHANGES.tsv').read().splitlines()[1:]:
    f=l.split('\t'); k=(f[1],f[2])
    if f[0]=='delete': T.pop(k,None)
    elif f[0] in ('add','rewitness'): T[k]=f[3]
    elif f[0]=='replace':
        o,n=[x.strip() for x in f[1].split('->')]; T.pop((o,f[2]),None); T[(n,f[2])]=f[3]
E={l.split('\t')[0]:l.split('\t')[6] for l in open(A+'ENTRIES.tsv').read().splitlines()[1:]}
E['008e9710']='?ClearStrafingCombinationAnims@CModeStrafe@NCreatureMode@@AAEX_N@Z'
for l in open(A+'STATUS.tsv').read().splitlines()[1:]:
    f=l.split('\t'); t=f[0].replace('0x','')
    if not f[1].startswith('done'): continue
    rest=[(s,w) for (s,tt),w in T.items() if tt==t and s!=E[t]]
    print(f[1],f[0],'PDBline=%s'%((E[t],t) in T),'; '.join('%s (%s)'%(s[:70],w) for s,w in rest))
