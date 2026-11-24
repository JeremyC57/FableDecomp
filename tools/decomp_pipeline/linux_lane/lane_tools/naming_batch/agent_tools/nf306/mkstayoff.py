# mkstayoff.py: NF306 stay-off sets -> work/stayoff.txt (= stayoff2.txt) and work/t3rows.txt.
#   stay-off = NF304 / NF305 ENTRIES targets + every witness address in their spellings column + addresses locked by anyone but NF306
#              + the standing held rows of BRIEF_NF_BATCH + Codex pass 28 rows still reserved.
#   t3rows   = every address in naming/T3_PORT_QUEUE.tsv.
import re,os
S='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad'; A=S+'/w22/agentNF306'
R='/home/user/FableDecomp/rebuild/candidates/manual/'
so=set(); why={}
def add(a,w):
    a=a.lower().replace('0x','')
    if re.fullmatch(r'[0-9a-f]{8}',a): so.add(a); why.setdefault(a,w)
for ag in ('NF304','NF305'):
    for l in open(S+'/w22/agent%s/ENTRIES.tsv'%ag).read().splitlines()[1:]:
        c=l.split('\t'); add(c[0],ag+'-entry')
        for w in re.findall(r'\(([0-9a-f]{8})(?:,self)?\)',c[11] if len(c)>11 else ''): add(w,ag+'-witness')
# NF306 fix: ENTRIES column 12 is truncated for big entries, so take every witness of their targets from reloc_bindings.tsv
TG={'NF304':set(),'NF305':set()}
for ag in TG:
    for l in open(S+'/w22/agent%s/ENTRIES.tsv'%ag).read().splitlines()[1:]: TG[ag].add(l.split('\t')[0].lower())
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3:
        for ag in TG:
            if r[1].lower() in TG[ag]: add(r[2],ag+'-witness(table)')
# and every row they changed or listed (rows/<addr>.cpp, rows.txt)
for ag in TG:
    d=S+'/w22/agent%s/rows'%ag
    if os.path.isdir(d):
        for f in os.listdir(d):
            m=re.match(r'([0-9a-fA-F]{8})',f)
            if m: add(m.group(1),ag+'-row')
    rt=S+'/w22/agent%s/rows.txt'%ag
    if os.path.exists(rt):
        for a in open(rt).read().split(): add(a,ag+'-row')
for d in os.listdir(S+'/w22/nflocks'):
    try: o=open(S+'/w22/nflocks/%s/owner'%d).read().strip()
    except Exception: continue
    if o not in ('released','NF306'): add(d,'lock-'+o)
for a in '00B4D370 00BD3070 00890250 0091C080 0052C7E0 00A254A0 00ADC190 00ADC250 00ADC4E0 00ADCB90 00ADCE60 00BE8E60 00BFBD80 00BE6090 00BFB590'.split(): add(a,'standing-held')
for l in open(R+'pending/2026-11-22_codex_pass28/RESERVED.tsv').read().splitlines()[1:]:
    c=l.split('\t')
    if 'no longer reserved' not in l: add(c[0],'p28-reserved')
open(A+'/work/stayoff.txt','w').write('\n'.join(sorted(so))+'\n'); open(A+'/work/stayoff2.txt','w').write('\n'.join(sorted(so))+'\n')
open(A+'/work/stayoff_why.tsv','w').write(''.join('%s\t%s\n'%(a,why[a]) for a in sorted(so)))
t3=set()
for l in open(R+'naming/T3_PORT_QUEUE.tsv').read().splitlines()[1:]:
    c=l.split('\t')
    if len(c)>1:
        for a in re.findall(r'0x([0-9A-Fa-f]{8})',c[1]): t3.add(a.lower())
open(A+'/work/t3rows.txt','w').write('\n'.join(sorted(t3))+'\n')
print('stayoff',len(so),'t3',len(t3))
