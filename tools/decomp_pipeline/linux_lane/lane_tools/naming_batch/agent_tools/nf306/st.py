# st.py <entry#> <status> <binder_rows> <grade> <note>: set one STATUS.tsv line (keyed by entry number; ENTRIES order kept)
import sys,os
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF306/'
L=[l.split('\t') for l in open(A+'ENTRIES.tsv').read().splitlines()]
p=A+'STATUS.tsv'; d={}
if os.path.exists(p):
    for l in open(p).read().splitlines()[1:]:
        c=l.split('\t'); d[int(c[0])]=c
n=int(sys.argv[1]); d[n]=[str(n),'0x'+L[n][0]]+sys.argv[2:6]
with open(p,'w') as f:
    f.write('entry\taddress\tstatus\tbinder_rows\tgrade\tnote\n')
    for k in sorted(d): f.write('\t'.join(d[k])+'\n')
