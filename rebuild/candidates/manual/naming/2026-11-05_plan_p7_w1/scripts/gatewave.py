# gatewave.py W [addr...] : run $S/plan/gate.py on every row of wave W (overlay sources; header overlay = W/overlay/rebuild/include
# when present), in batches of 200 with NJ=2, into W/gate.tsv (gate.py's columns). Rows already in W/gate.tsv are skipped unless
# --redo. Prints the verdict counter.  Usage: python3 gatewave.py W [--redo] [addr...|@file]
import sys,os,subprocess,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__))); import wavelib as L
W=sys.argv[1]; args=sys.argv[2:]; redo='--redo' in args; args=[a for a in args if a!='--redo']
addrs=[]
for a in args: addrs+=[l.split()[0] for l in open(a[1:]) if l.strip() and not l.startswith('address')] if a.startswith('@') else [a]
if not addrs: addrs=L.wave_rows(W)
done={}
if os.path.exists(W+'/gate.tsv'):
    for r in L.tsv(W+'/gate.tsv'): done[r['address']]=r
if redo:
    for a in addrs: done.pop(a,None)
todo=[a for a in addrs if a not in done]
hdr=os.path.join(W,'overlay','rebuild','include'); ov=['--overlay',hdr] if os.path.isdir(hdr) else []
os.makedirs(W+'/gate_batches',exist_ok=True); env=dict(os.environ,NJ=os.environ.get('NJ','2'))
for i in range(0,len(todo),200):
    b=todo[i:i+200]; lst=W+'/gate_batches/b%04d.lst'%i; out=W+'/gate_batches/b%04d.tsv'%i
    with open(lst,'w') as o:
        for a in b:
            p=L.ov_path(W,L.src_rel(a)); o.write(a+'\t'+(p if os.path.exists(p) else '-')+'\n')
    if os.path.exists(out): os.remove(out)   # phase 7: never read a stale batch file when gate.py fails
    rc=subprocess.run(['python3',L.S+'/plan/gate.py']+ov+['--work',L.P+'/gate_work','--out',out,'@'+lst],env=env,cwd=L.ROOT).returncode
    got=L.tsv(out) if os.path.exists(out) else []
    if rc or len(got)!=len(b): print('gatewave: batch %s returned %d rows of %d (rc %d)'%(lst,len(got),len(b),rc))
    for r in got: done[r['address']]=r
with open(W+'/gate.tsv','w') as o:
    o.write('address\tverdict\treason\tchanges\tname_notes\tsecs\n')
    for a in sorted(done): r=done[a]; o.write('\t'.join(r.get(k,'') for k in ('address','verdict','reason','changes','name_notes','secs'))+'\n')
c=collections.Counter(done[a]['verdict'] for a in addrs if a in done); print('gatewave',W,dict(c))
