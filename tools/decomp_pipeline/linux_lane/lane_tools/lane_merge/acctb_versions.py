"""acctb_versions.py <header>: the distinct overlay versions of a header across the unlanded acctB rows, largest first, with which rows carry
each and whether each smaller version's lines are all contained in the largest."""
import json,os,sys,hashlib,difflib
S=os.environ['S']; B=S+'/w22/acctb'; h=sys.argv[1]; left=json.load(open(B+'/left.json'))
landed=set(l.split('\t',1)[0].lower() for l in open('rebuild/oracles/auto-re-candidates.tsv').read().splitlines()[1:])
v={}
for a in left:
    if a in landed: continue
    p=f'{B}/{a}/hdr/{h}'
    if os.path.exists(p):
        t=open(p,errors='replace').read(); k=hashlib.sha1(t.encode()).hexdigest()[:8]; v.setdefault(k,[t,[]])[1].append(a)
vs=sorted(v.items(),key=lambda x:-len(x[1][0]))
if not vs: print('no overlay copies'); sys.exit(0)
big=vs[0][1][0].split('\n')
for k,(t,rows) in vs:
    L=t.split('\n'); miss=0
    for op,a1,a2,b1,b2 in difflib.SequenceMatcher(None,L,big,autojunk=False).get_opcodes():
        if op in('delete','replace'): miss+=a2-a1
    print(k,len(L),'lines | lines not in the largest:',miss,'| rows:',' '.join(rows))
