# unmatched23.py <codex_x>: update rebuild/candidates/manual/UNMATCHED_ROWS.tsv after landing p23
import csv,sys
X=sys.argv[1]; B='rebuild/candidates/manual/batches/2026-11-16_landing_p23'; O='rebuild/candidates/manual/pending/2026-11-16_codex_pass23_open'
st={r['address'][2:].lower():r for r in csv.DictReader(open(X+'/STATUS.tsv'),delimiter='\t')}
landed=set()
for r in csv.DictReader(open(B+'/LANDED.tsv'),delimiter='\t'):
    landed.add(r['address']); landed.add(r['reserved_as'])
landed|={'004a1c41','008f92ef'}            # inside the landed extents of 0x004A1840 / 0x008F8F50
held={'00c29a3c':('00c29a3c','RELOCATION_MATCH (Codex pass 23); needs Dinkumware <vector> include order the harness lacks'),
      '00c298d0':('00c29a3c','RELOCATION_MATCH (Codex pass 23, whole parent in the 0x00C29A3C draft); needs Dinkumware <vector> include order'),
      '00c29a30':('00c29a3c','inside the 0x00C298D0 extent; lands with that parent'),
      '01210e95':('01210e95','RELOCATION_MATCH (Codex pass 23); naming-blocked (Unproven placeholder class names)'),
      '00c0af90':('01210e95','RELOCATION_MATCH (Codex pass 23, whole parent in the 0x01210E95 draft); naming-blocked'),
      '01212472':('01212472','RELOCATION_MATCH (Codex pass 23); naming-blocked (Unproven placeholder class names)'),
      '00c1c460':('01212472','RELOCATION_MATCH (Codex pass 23, whole parent in the 0x01212472 draft); naming-blocked')}
P='rebuild/candidates/manual/UNMATCHED_ROWS.tsv'
L=[l.rstrip('\n').split('\t') for l in open(P,encoding='utf-8')]
hdr,rows=L[0],L[1:]; out=[hdr]; rm=0; ch=0
for r in rows:
    a=r[0].lower()
    if a in landed: rm+=1; continue
    if a in held:
        f,why=held[a]; r[4]='parked'; r[5]=why; r[6]=str(int(r[6] or 0)+1); r[7]=f'{O}/{f}/{f}.cpp'; ch+=1
    elif 'pass23' in r[4]:
        s=st.get(a)
        if s and s['status']=='PARKED':
            r[4]='parked'; r[5]='Codex pass 23: '+(s.get('residue') or s.get('note') or 'PARKED')[:120]; r[6]=str(int(r[6] or 0)+1)
            r[7]=f'{O}/{a}/{a}.cpp'; ch+=1
        else:
            r[4]='never_worked' if 'never_worked' in r[4] else r[4].split('(')[0].strip() or 'parked'; ch+=1
    out.append(r)
open(P,'w',encoding='utf-8').write('\n'.join('\t'.join(r) for r in out)+'\n')
print('removed',rm,'changed',ch,'left pass23',sum(1 for r in out if 'pass23' in r[4]))
