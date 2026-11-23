# cls2.py: from work/cls1.tsv SAMESCOPE / FREE lines, compare the signature tail (after the scope's '@@') of stand-in and PDB:
# SIGEQ = identical tail (only the leaf differs), ACCESS = tail equal after the access/const code, else SIGDIFF.
import re,sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF296'
T={}
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: T.setdefault(r[1].lower(),[]).append((r[0],r[2]))
L=[l.split('\t') for l in open(A+'/ENTRIES.tsv').read().splitlines()]
def tail(s):
    m=re.match(r'\?[^@]+@(.*?)@@(.*)$',s)
    return (m.group(1),m.group(2)) if m else (None,None)
for n in range(1,len(L)):
    c=L[n]; pdb=c[6]; ps,pt=tail(pdb)
    for s,w in T.get(c[0].lower(),[]):
        if s==pdb: continue
        ss,st=tail(s)
        if ps is None or ss!=ps: continue
        if st==pt: k='SIGEQ'
        elif st[1:]==pt[1:] or st[2:]==pt[2:]: k='ACCESS'
        else: k='SIGDIFF'
        print('%d\t%s\t%s\t%s\t%s\t%s'%(n,c[0],k,w,s[:120],pdb[:120]))
