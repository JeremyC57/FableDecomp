# mdiff.py <from> <to>: per (entry, stand-in, witness) classify how the stand-in mangled name differs from the PDB spelling:
#   SCOPE (same leaf, other scope, same signature), QUAL (same leaf+scope, only the access/const/virtual letters differ),
#   SCOPE+QUAL, SIG (same leaf+scope+qual, signature differs: params / return), CLASS (other class token), FREE (stand-in is a free
#   function or C stub), OTHER. Plus the off-limits reasons of target and witness (work/offwhy.tsv).
import re,sys,collections
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF308'
why=collections.defaultdict(set)
for l in open(A+'/work/offwhy.tsv'):
    a,r=l.split(); why[a].add(r)
def split(sym):
    if not sym.startswith('?') or sym.startswith('??$'): return None
    m=re.match(r'\?(\?_[0-9A-Z]|\?[0-9A-Z]|[A-Za-z_]\w*@)(.*)$',sym)
    if not m: return None
    leaf,rest=m.group(1).rstrip('@'),m.group(2)
    i=rest.find('@@')
    if i<0: return None
    scope=rest[:i]; tail=rest[i+2:]
    q=re.match(r'([A-Z]|\$[A-Z]?)([A-Z]?)(.*)',tail)
    if not q: return None
    acc=q.group(1)
    if acc in 'YZ': return leaf,scope,acc,'',q.group(2)+q.group(3)
    if acc in 'STU' or acc in 'CDK': return leaf,scope,acc,'',q.group(2)+q.group(3)   # static: no this-qualifier
    return leaf,scope,acc,q.group(2),q.group(3)
L=open(A+'/ENTRIES.tsv').read().splitlines()
for n in range(int(sys.argv[1]),int(sys.argv[2])+1):
    c=L[n].split('\t'); t,pdb,sp=c[0],c[6],c[11]; P=split(pdb)
    for part in sp.split(' || '):
        m=re.match(r'(\S+) \(([^)]*)\)',part)
        if not m: continue
        sym,wit=m.group(1),m.group(2)
        if sym==pdb: continue
        Q=split(sym)
        if not Q or not P: v='FREE' if (sym.startswith('@') or sym.startswith('_')) else 'OTHER'
        else:
            ls,ss,as_,qs,gs=Q; lp,sp_,ap,qp,gp=P
            if ls!=lp: v='LEAF'
            elif ss==sp_:
                v='QUAL' if gs==gp else ('SIG+QUAL' if (as_,qs)!=(ap,qp) else 'SIG')
            else:
                cs=ss.split('@')[0]; cp=sp_.split('@')[0]
                if as_ in 'YZ' and ap not in 'YZ': v='FREE'
                elif cs!=cp: v='CLASS'
                else: v='SCOPE' if gs==gp and (as_,qs)==(ap,qp) else ('SCOPE+QUAL' if gs==gp else 'SCOPE+SIG')
        for w in wit.split(','):
            if w=='...' : continue
            print('#%d\t%s\t%s\t%s\tT:%s\tW:%s\t%s'%(n,t,w,v,','.join(sorted(why[t])) or '-',','.join(sorted(why[w])) or '-',sym[:110]))
