# rec.py <specfile> (NF303: RENAMES caller old new callee; TABLE_CHANGES bare addresses, rewitness = the new witness): append hand-back records from a spec (one line per (entry, binder): entry target binder old_symbol new_symbol status note,
# tab-separated, `-` for none). Writes APPLIED.tsv / RENAMES.tsv section 2 lines for changed binders and TABLE_CHANGES.tsv lines:
#   delete <old> when every binder that names it is in the spec as changed and no left binder keeps it; rewitness when the table witness
#   is changed but a left binder keeps it; add <new> when (new,target) has no table line. Idempotent per spec (skips lines already present).
import sys,os
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF303/'
T={}
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: T[(r[0],r[1].lower())]=r[2]
spec=[l.rstrip('\n').split('\t') for l in open(sys.argv[1]) if l.strip() and not l.startswith('#')]
def app(f,line):
    p=A+f; have=open(p).read().split('\n') if os.path.exists(p) else []
    if line not in have: open(p,'a').write(line+'\n')
chg={}; left={}
for n,t,b,old,new,st,note in spec:
    if st=='changed':
        app('work/applied_perbinder.tsv','\t'.join(['0x'+t,'0x'+b,old,new,note]))
        app('RENAMES.tsv','\t'.join(['0x'+b,old,new,'0x'+t]))
        chg.setdefault((old,t),set()).add(b); chg.setdefault(('NEW',new,t),set()).add(b)
    else: left.setdefault((old,t),set()).add(b)
for k,bs in chg.items():
    if k[0]=='NEW':
        _,new,t=k
        if (new,t) not in T: app('TABLE_CHANGES.tsv','\t'.join(['add',new,t,sorted(bs)[0],'binder respelled by NF303']))
        continue
    old,t=k; w=T.get((old,t))
    if w is None: continue
    if (old,t) in left and left[(old,t)]:
        if w in bs: app('TABLE_CHANGES.tsv','\t'.join(['rewitness',old,t,sorted(left[(old,t)])[0],'was '+w+'; witness respelled; binder left: '+','.join(sorted(left[(old,t)]))]))
    else: app('TABLE_CHANGES.tsv','\t'.join(['delete',old,t,w,'no landed binder left (NF303)']))
print(open(A+'TABLE_CHANGES.tsv').read())
