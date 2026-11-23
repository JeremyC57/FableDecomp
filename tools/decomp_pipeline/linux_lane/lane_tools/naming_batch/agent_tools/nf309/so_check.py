# so_check.py: per ENTRIES line: target stay-off reason; table witnesses (reloc_bindings, full, not the truncated ENTRIES column) and
# landed exe caller rows (work/xs_all.tsv) that are in stay-off / T3. NF309.
import re
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF309'
why=dict(l.split('\t') for l in open(A+'/work/stayoff_why.tsv').read().splitlines())
t3=set(open(A+'/work/t3rows.txt').read().split()); t3m=set(open(A+'/work/t3mention.txt').read().split())-t3
T={}
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: T.setdefault(r[1].lower(),[]).append((r[0],r[2].lower()))
xs={}
for l in open(A+'/work/xs_all.tsv'):
    p=l.rstrip('\n').split('\t'); xs.setdefault(p[0],set()).add(p[3][:9] if p[3]!='-' else '-')
L=[l.split('\t') for l in open(A+'/ENTRIES.tsv').read().splitlines()]
for n in range(1,len(L)):
    c=L[n]; t=c[0]; P=c[6]
    ws={w:s for s,w in T.get(t,[])}
    r=xs.get(t,set()); cr=set(x[:8] for x in r if x!='-' and x[8]=='L')
    f=['W:%s=%s'%(w,why[w]) for w in sorted(ws) if w in why and ws[w]!=P]+['W-PDB:%s=%s'%(w,why[w]) for w in sorted(ws) if w in why and ws[w]==P]
    f+=['T3W:%s'%w for w in sorted(ws) if w in t3]+['T3MW:%s'%w for w in sorted(ws) if w in t3m]
    f+=['C:%s=%s'%(a,why[a]) for a in sorted(cr) if a in why and a not in ws]+['T3C:%s'%a for a in sorted(cr) if a in t3 and a not in ws]+['T3MC:%s'%a for a in sorted(cr) if a in t3m and a not in ws]
    print('%d\t%s\t%s\t%s\tcallers_landed=%d/%d\t%s'%(n,t,why.get(t,'-'),'T3' if t in t3 else ('T3M' if t in t3m else '-'),len(cr),len(r),' '.join(f)))
