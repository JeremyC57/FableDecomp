# tri5.py: per filt.tsv line (CLEAN / PRIV): target length, stand-in prefix ?Ident@Scope@@ in Ego_r / FableWin name sets (an ICF-fold
# hint when present), the PDB spelling in both sets, stay-off (NF293 / NF295 / locks / T3 queue) for target and binder.
import re,csv,sys
csv.field_size_limit(10**9)
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF314'
R='/home/user/FableDecomp/'
eg=open(R+'ghidra_out/egor_pdb_names.tsv').read(); fw=open(R+'ghidra_out/fablewin_pdb_names.tsv').read()
so=set(open(A+'/work/stayoff.txt').read().split())
t3=set(re.findall(r'0x([0-9A-Fa-f]{8})',open(R+'rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv').read())); t3={x.lower() for x in t3}
ln={}
for l in open(R+'rebuild/oracles/auto-re-candidates.tsv',encoding='utf-8'):
    r=l.split('\t',3)
    if len(r)>2 and r[2].isdigit(): ln[r[0].lower().replace('0x','')]=int(r[2])
L=[l.split('\t') for l in open(A+'/ENTRIES.tsv').read().splitlines()]
for l in open(A+'/work/filt.tsv'):
    n,t,w,ident,cls,fl,pl=l.rstrip('\n').split('\t')
    if fl not in ('CLEAN','PRIV'): continue
    pdb=L[int(n)][6]; m=re.match(r'\?[^@]+@(.*?)@@',pdb); scope=m.group(1) if m else ''
    pre='?%s@%s@@'%(ident,scope)
    st=('E' if pre in eg else '-')+('F' if pre in fw else '-')
    pp=('E' if pdb in eg else '-')+('F' if pdb in fw else '-')
    flg=[]
    if t in so: flg.append('T_STAYOFF')
    if w in so: flg.append('B_STAYOFF')
    if t in t3: flg.append('T_T3')
    if w in t3: flg.append('B_T3')
    print('\t'.join([n,t,str(ln.get(t,'?')),w,ident,pl,fl,'standin='+st,'pdb='+pp,' '.join(flg)]))
