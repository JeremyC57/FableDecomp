# mkapp.py: rebuild APPLIED.tsv from STATUS.tsv in ENTRIES order (entry index, address, status, binders respelled, note head);
# entries not reached are `open`; STATUS lines for addresses outside ENTRIES are appended with index '-'
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF294'
E=[l.split('\t')[0] for l in open(A+'/ENTRIES.tsv').read().splitlines()[1:]]
S={}
for l in open(A+'/STATUS.tsv').read().splitlines()[1:]:
    c=l.split('\t'); S[c[0].replace('0x','')]=c
o=['entry\taddress\tstatus\tbinders_respelled\tnote']
for i,a in enumerate(E,1):
    c=S.get(a)
    if not c: o.append('%d\t0x%s\topen\t0\tnot started'%(i,a)); continue
    nb=len([x for x in c[2].split(',') if x.startswith('0x')]) if c[1].startswith('done') and c[1]!='done-already' and 'table only' not in c[1] else 0
    o.append('%d\t0x%s\t%s\t%d\t%s'%(i,a,c[1],nb,c[4][:240]))
for a,c in S.items():
    if a not in E: o.append('-\t0x%s\t%s\t%d\t%s'%(a,c[1],len([x for x in c[2].split(',') if x.startswith('0x')]),c[4][:240]))
open(A+'/APPLIED.tsv','w').write('\n'.join(o)+'\n')
import collections; print(collections.Counter(x.split('\t')[2] for x in o[1:]))
