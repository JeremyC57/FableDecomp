# sources of held rows whose bytes match: 43 Codex pass 30 hand-back, 16 A_MATCHED_HOLD, 6 parked
import csv,json,os,re,sys
csv.field_size_limit(sys.maxsize)
S=sys.argv[1]
R=list(csv.DictReader(open('rebuild/candidates/manual/UNMATCHED_NOW.tsv'),delimiter='\t'))
um={r['address']:r for r in csv.DictReader(open('rebuild/candidates/manual/UNMATCHED_ROWS.tsv'),delimiter='\t')}
picked={p[0]:p for p in json.load(open(S+'/p31/picked.json'))}
ov={}
import glob
for g in glob.glob(S+'/p31/g/g[0-9]'):
    o=open(g+'/overlay').read()
    for r in json.load(open(g+'/payload.json'))['result']['authored']: ov[r['address']]=o
out=[]
for r in R:
    if not r['body'].startswith('matches, held'): continue
    a=r['address']; src=''; o=''
    if r['group'].startswith('reserved for Codex (pass 30'): src=f"{S}/p30in/rows/{a}/{a}.cpp"
    elif r['group'].startswith('matches at base'): src=re.sub(r'\.test\.cpp$','.cpp',picked[a][2]); o=ov.get(a,'')
    else:
        src=re.sub(r'\.test\.cpp$','.cpp',um.get(a,{}).get('closest_version',''))
    out.append(dict(address=a,group=r['group'],name=r['name'],source=src,overlay=o,exists=os.path.exists(src)))
json.dump(out,open(S+'/inv/held.json','w'),indent=0)
print(len(out),sum(x['exists'] for x in out)); [print(x['address'],x['source'][-70:]) for x in out if not x['exists']]
