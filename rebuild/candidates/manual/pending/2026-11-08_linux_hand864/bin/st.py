# st.py: rebuild STATUS.tsv from tmp/attempts.log (+ ATTEMPTS.log copy); best = lowest md (ties: fewest bytes)
import os,re,csv
S=os.environ['S']; A=f"{S}/w22/agent864"
names={r['address']:r['name'] for r in csv.DictReader(open(f"{S}/batch/chunk864_oracle.tsv",encoding='utf-8-sig'),delimiter='\t')}
rows={}
for line in open(f"{A}/tmp/attempts.log"):
    p=line.rstrip('\n').split('\t')
    if len(p)<6: continue
    t,addr,tag,st,md,by=p[:6]; note=p[7] if len(p)>7 else ''
    md=int(md.split('=')[1]) if '=' in md else 999
    if md<0 or st=='SRC_FAIL': md=999; by=by.split('=')[1].split()[0]; by=int(by) if by.lstrip('-').isdigit() else 9999
    r=rows.setdefault(addr,{'n':0,'best':None,'notes':[]})
    r['n']+=1; r['notes'].append(f"{tag}:{st}/md{md}")
    key=(0 if st.startswith(('MATCH','RELOCATION')) else 1, md, by)
    if r['best'] is None or key<r['best'][0]: r['best']=(key,tag,st,md,by)
with open(f"{A}/STATUS.tsv","w") as f:
    f.write("address\tname\tstatus\tattempts\tbest\tgrade\tdifflines\tfile\tnotes\n")
    for a in names:
        r=rows.get(a)
        if not r: f.write(f"{a}\t{names[a]}\tuntouched\t0\t-\t-\t-\tsrc/{a}.cpp\t\n"); continue
        k,tag,st,md,by=r['best']; status='matched' if k[0]==0 else 'parked'
        f.write(f"{a}\t{names[a]}\t{status}\t{r['n']}\t{tag}\t{st}\t{md}\t{'src' if status=='matched' else 'differ'}/{a}.cpp\t{' '.join(r['notes'][-12:])}\n")
print(open(f"{A}/STATUS.tsv").read())
