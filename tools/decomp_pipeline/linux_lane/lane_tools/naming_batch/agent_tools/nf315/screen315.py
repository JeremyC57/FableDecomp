# screen315.py: per ENTRIES line, the target's off-limits reasons and every reloc_bindings spelling of it with its witnesses' reasons
import sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF315'
why={}
for l in open(A+'/work/offwhy.tsv'):
    a,r=l.split(); why.setdefault(a,[]).append(r)
tb={}
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: tb.setdefault(r[1].lower(),[]).append((r[0],r[2]))
L=open(A+'/ENTRIES.tsv').read().splitlines()[1:]
for i,l in enumerate(L,1):
    c=l.split('\t'); t=c[0]; pdb=c[6]
    print('L%d %s %s off=%s leaf=%s'%(i,t,c[3],','.join(why.get(t,[])) or '-', 'self' if c[7]==pdb else (c[7] or 'none')))
    print('   PDB %s'%pdb)
    for s,w in tb.get(t,[]):
        tag='==PDB' if s==pdb else '    '
        print('   %s %s  W=%s %s'%(tag,s[:200],w,' '.join('[%s:%s]'%(x,','.join(why[x.lower().replace('0x','')])) for x in w.split(',') if x.lower().replace('0x','') in why)))
