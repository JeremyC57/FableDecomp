# delaudit3.py <plan_lines.tsv> <audit_pat.tsv>: exe-based delete audit (advisor, NF314). For each planned delete / replace line (target,
# old symbol, witness, entry): every landed row that calls / jumps to the target (xsites) or holds an absolute reference to it (absrows),
# its current source (rows/<addr>.cpp when this hand-back changed it, else the repo row), grepped (comments stripped) for the entry's
# old-declaration pattern. Prints HIT lines (row, line, text) for reading; rows without a hit cannot bind the old spelling by that form.
import sys,re,subprocess,os
A=os.path.dirname(os.path.abspath(__file__))+'/..'
pat={l.split('\t')[0]:re.compile(l.rstrip('\n').split('\t')[1]) for l in open(sys.argv[2]) if l.strip()}
def src(a):
    if os.path.exists(A+'/rows/%s.cpp'%a): return A+'/rows/%s.cpp'%a
    return subprocess.run(['bash',A+'/tools/srcof.sh',a],capture_output=True,text=True).stdout.strip()
cache={}
for l in open(sys.argv[1]):
    t,s,w,n=l.rstrip('\n').split('\t')
    if t not in cache:
        xs=subprocess.run(['python3',A+'/tools/xsites.py',t],capture_output=True,text=True).stdout.splitlines()
        ab=subprocess.run(['python3',A+'/tools/absrows.py',t],capture_output=True,text=True).stdout.splitlines()
        rows=set(); dat=0
        for x in xs+ab:
            c=x.split('\t'); r=c[-1]
            if r=='-': dat+=1; continue
            if r[8]=='L': rows.add(r[:8])
        cache[t]=(sorted(rows),dat)
    rows,dat=cache[t]; hits=[]
    for r in rows:
        f=src(r)
        if not f or not os.path.exists(f): continue
        for i,line in enumerate(open(f,encoding='utf-8',errors='replace')):
            code=line.split('//')[0]
            if pat[n].search(code): hits.append('%s:%d %s'%(r,i+1,code.strip()[:150]))
    print('## entry %s %s old=%s witness=%s landed_rows=%d data_refs=%d hits=%d'%(n,t,s[:90],w,len(rows),dat,len(hits)))
    for h in hits: print('   HIT',h)
