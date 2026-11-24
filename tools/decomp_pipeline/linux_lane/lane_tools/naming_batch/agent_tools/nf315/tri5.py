# tri5.py <from> <to>: tri4 verdicts per (entry, stand-in, witness) plus the off-limits reason of the target and the witness
import subprocess,sys,collections
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF315'
why=collections.defaultdict(set)
for l in open(A+'/work/offwhy.tsv'):
    a,r=l.split(); why[a].add(r)
out=subprocess.run(['python3',A+'/tools/tri4.py']+sys.argv[1:3],capture_output=True,text=True).stdout
for l in out.splitlines():
    c=l.split('\t'); t,w=c[1],c[2]
    print('%s\tT:%s\tW:%s\t%s'%(l[:230],','.join(sorted(why[t])) or '-',','.join(sorted(why[w])) or '-',''))
