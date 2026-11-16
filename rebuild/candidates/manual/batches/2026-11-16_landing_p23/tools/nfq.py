import csv,json,os,re
S=os.environ['S']; q='rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv'
Q={}
for l in open(q,encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)>2: Q.setdefault(r[1].lower(),set()).add(r[2])
leaf={e['address']:e['leaf'] for e in json.load(open(S+'/p23l/payload.json'))['result']['authored'] if e.get('funclet_offset') is None}
lab={}
for l in open('ghidra_out/labels_landing_2026-11-16gn.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)>1 and re.fullmatch(r'(0x)?[0-9a-fA-F]{8}',r[0]): lab[r[0].lower().replace('0x','')]=r
add=[];conf=[];dup=0
for r in csv.DictReader(open(S+'/codex_x/NAME_FIXES.tsv',encoding='utf-8'),delimiter='\t'):
    a=r['address'].lower(); a8=a.replace('0x',''); sym=r['correct_symbol']
    have=Q.get('0x'+a8,set())|Q.get(a8,set())
    if sym in have: dup+=1; continue
    why=[]
    if have: why.append('queued differently: '+' | '.join(sorted(have)))
    if a8 in leaf and leaf[a8]!=sym: why.append('pass-23 landed leaf '+leaf[a8])
    if a8 in lab: why.append('2026-11-16gn label '+' '.join(lab[a8][1:3]))
    row=['land1116p23','0x'+a8.upper(),sym,r['evidence'],r['wrong_spellings_seen']]
    (conf if why else add).append(row+(['; '.join(why)] if why else []))
print('dup',dup,'add',len(add),'conflicts',len(conf))
with open(S+'/p23l/nf_add.tsv','w') as f:
    for x in add: f.write('\t'.join(x)+'\n')
with open(S+'/p23l/nf_conflicts.tsv','w') as f:
    f.write('tag\taddress\tcodex_symbol\tevidence\twrong_spellings_seen\twhy_held\n')
    for x in conf: f.write('\t'.join(x)+'\n')
