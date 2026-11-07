# st.py: rebuild STATUS.tsv from tmp/attempts.log (+ tmp/rowstate.tsv: addr<TAB>status<TAB>file<TAB>summary)
import os,csv,collections
A=os.environ['S']+'/w22/agent860'
names={r['address'].lower():r['name'] for r in csv.DictReader(open(os.environ['S']+'/batch/chunk860_oracle.tsv',encoding='utf-8-sig'),delimiter='\t')}
log=collections.defaultdict(list)
for l in open(A+'/tmp/attempts.log'):
    p=l.rstrip('\n').split('\t')
    if len(p)<8: continue
    log[p[1]].append(p)
state={}
if os.path.exists(A+'/tmp/rowstate.tsv'):
    for l in open(A+'/tmp/rowstate.tsv'):
        p=l.rstrip('\n').split('\t'); state[p[0]]=p
rows=['address\tname\tstatus\tattempts\tbest\tgrade\tdifflines\tfile\tnotes']
for a in names:
    L=log.get(a,[]); real=[p for p in L if 'DIAG' not in p[7] and p[2]!='base' and p[3]!='SRC_FAIL']
    def key(p):
        md=int(p[4][3:]); nb=int(p[5][6:].split(' ')[0])
        ok=p[3].startswith(('MATCH','RELOCATION'))
        return (0 if ok else 1, md if md>=0 else 9999, nb if nb>=0 else 99999)
    best=min([p for p in L if "DIAG" not in p[7] and "diag" not in p[7]] or L,key=key) if L else None
    real=[p for p in real if "diag" not in p[7]]
    st=state.get(a,[a,'WORKING','src/%s.cpp'%a,''])
    notes=st[3]+' | attempts: '+'; '.join(f"{p[2]}:{p[3]} md{p[4][3:]} b{p[5][6:]} ({p[7]})" for p in L)
    rows.append('\t'.join([a,names[a],st[1],str(len(real)),best[2] if best else '',best[3] if best else '',best[4][3:] if best else '',st[2],notes]))
open(A+'/STATUS.tsv','w').write('\n'.join(rows)+'\n')
print('\n'.join(r[:160] for r in rows))
