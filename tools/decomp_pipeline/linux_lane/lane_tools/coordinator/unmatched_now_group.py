# unmatched_now_group.py <S>: group $S/um/UNMATCHED_NOW.tsv (unmatched_now_build.py) into rebuild/candidates/manual/UNMATCHED_NOW.tsv,
# with a body column (does the row's best code match retail?) read from the status, the grade on record and the pass 31 base regrade.
import csv,sys,re,json,os,collections
csv.field_size_limit(sys.maxsize)
S=sys.argv[1]; R=list(csv.DictReader(open(S+'/um/UNMATCHED_NOW.tsv'),delimiter='\t'))
G={}
gp='rebuild/candidates/manual/pending/2026-11-28_codex_pass31/GRADES_AT_BASE.json'
if os.path.exists(gp): G=json.load(open(gp))['grades']
INTERIOR={'004a1c41','008f92ef','00ba115c','00bc3f3d','00bd71cc','00564d5d','00d2988b','0074bc30','00537e70'}
def body(r):
    a=r['address']; t=(r['best_grade']+' '+r['reserve_note']).upper()
    if r['status'].startswith('landed'): return 'matches (landed)'
    if a in INTERIOR: return 'not a function (inside a landed extent)'
    if a in G: return 'matches, held' if G[a].startswith(('MATCH','RELOCATION')) else 'differs at base: '+G[a]
    if re.search(r'EXACT|RELOCATION_MATCH|BYTE_EXACT|\bMATCH\b',t) and 'DIFFER' not in r['reserve_note'].upper()[:40]: return 'matches, held'
    if 'DIFFER' in t or re.search(r'\d+v\d+',t): return 'differs'
    return 'not graded / unknown'
def grp(r):
    s=r['status']; a=r['address']
    if a in INTERIOR: return 'not a separate function'
    for k,v in (('locked by CODEX31','reserved for Codex (pass 31)'),('locked by CODEX30','reserved for Codex (pass 30 hand-back)'),
                ('locked by A_MATCHED_HOLD','matches at base, held by A for landing'),('UNMATCHED: parked','parked (in the pool)'),
                ('landed, baked','landed but baked data address'),('landed','landed, not genuine')):
        if s.startswith(k): return v
    if a=='009bef80': return 'reserved for Codex (pass 31)'
    return 'other ('+s[:40]+')'
for r in R: r['body']=body(r); r['group']=grp(r)
k=['address','name','group','body','status','best_grade','original','length','reserved_in','reserve_note','note']
w=csv.DictWriter(open('rebuild/candidates/manual/UNMATCHED_NOW.tsv','w',newline=''),fieldnames=k,delimiter='\t',extrasaction='ignore',lineterminator='\n')
w.writeheader(); [w.writerow(r) for r in sorted(R,key=lambda r:(r['group'],r['address']))]
c=collections.Counter(r['group'] for r in R)
for g,n in c.most_common(): print(n,g,dict(collections.Counter(re.sub(r':.*','',x['body']) for x in R if x['group']==g)))
