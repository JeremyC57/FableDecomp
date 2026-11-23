# mkstatus308.py: STATUS.tsv from tools/v308.tsv (entry_no, status, binders, grade, note) + ENTRIES.tsv addresses, in v308 order.
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF311'
E=open(A+'/ENTRIES.tsv').read().splitlines()
out=['entry\tstatus\tbinder_rows\tgrade\tnote']
seen=set()
for l in open(A+'/tools/v308.tsv'):
    if not l.strip() or l.startswith('#'): continue
    n,st,b,g,note=l.rstrip('\n').split('\t',4); n=int(n)
    assert n not in seen, n; seen.add(n)
    out.append('0x%s\t%s\t%s\t%s\tentry %d (ENTRIES line %d): %s'%(E[n].split('\t')[0],st,b,g,n,n+1,note))
open(A+'/STATUS.tsv','w').write('\n'.join(out)+'\n'); print(len(out)-1,'verdicts')
