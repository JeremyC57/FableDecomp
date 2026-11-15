# unmatched.py <S> <landed_reserved_addrs.txt> <handback_rel_dir>: update rebuild/candidates/manual/UNMATCHED_ROWS.tsv for pass 22
import csv,sys,json
S,landf,hb=sys.argv[1:4]; X=S+'/codex22_dl/x'
landed=set(open(landf).read().split())
st={r['address'][2:].lower():r for r in csv.DictReader(open(X+'/STATUS.tsv'),delimiter='\t')}
P='rebuild/candidates/manual/UNMATCHED_ROWS.tsv'
L=[l.rstrip('\n').split('\t') for l in open(P,encoding='utf-8')]
hdr,rows=L[0],L[1:]; out=[hdr]; rm=0; ch=0
for r in rows:
    a=r[0].lower()
    if a in landed: rm+=1; continue
    if 'pass22' in r[4]:
        s=st.get(a)
        if s is None: out.append(r); continue
        if s['status']=='PARKED':
            r[4]='parked'; r[5]='Codex pass 22: '+s['residue'][:120]; r[6]=str(int(r[6] or 0)+1)
            r[7]=f'{hb}/parked/{a}/{a}.cpp'
        elif s['status']=='EXTENT_WRONG':
            r[4]='parked'; r[5]='not a function start (Codex pass 22 EXTENT_WRONG; BOUNDS_FIX.tsv)'; r[7]=f'{hb}/extent_wrong/{a}/'
        elif s['status']=='RELOCATION_MATCH':
            r[4]='parked'; r[5]='RELOCATION_MATCH (Codex pass 22); behaviour test needs a port to the repo test harness'
            r[6]=str(int(r[6] or 0)+1); r[7]=f'{hb}/matched_test_port/{a}/{a}.cpp'
        ch+=1
    out.append(r)
open(P,'w',encoding='utf-8').write('\n'.join('\t'.join(r) for r in out)+'\n')
print('removed',rm,'changed',ch,'left pass22',sum(1 for r in out if 'pass22' in r[4]))
