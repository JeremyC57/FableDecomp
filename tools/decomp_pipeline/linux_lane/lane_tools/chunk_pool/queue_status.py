# usage: python3 queue_status.py   (repo root)
# Rewrite the generated part of docs/pipeline/FABLE_LANDING_QUEUE.md (between the queue markers) from
# rebuild/candidates/manual/pending/*/: one table row per staged batch, then each batch's parked rows copied
# from its NOTES.md "## Parked" section (the hand-over lines the agents wrote for the Fable 5.1 session).
import os,json,re,csv
root='rebuild/candidates/manual/pending'; doc='docs/pipeline/FABLE_LANDING_QUEUE.md'
rows=[]; parked=[]; tot_m=tot_p=0
for name in sorted(os.listdir(root)) if os.path.isdir(root) else []:
    p=f'{root}/{name}'
    staged=os.path.exists(p+'/payload.json')
    m=len(json.load(open(p+'/payload.json'))['result']['authored']) if staged else 0
    d=len([f for f in os.listdir(p+'/differ') if f.endswith('.cpp')]) if os.path.isdir(p+'/differ') else 0
    sk=0
    if os.path.exists(p+'/STATUS.tsv'):
        sk=sum(1 for r in csv.DictReader(open(p+'/STATUS.tsv'),delimiter='\t') if (r.get('status') or '').strip().upper()=='SKIP')
    notes=open(p+'/NOTES.md').read() if os.path.exists(p+'/NOTES.md') else ''
    title=(notes.splitlines() or [''])[0].lstrip('# ').strip()
    state='STAGED: land it' if staged else ('landed; parked rows open' if d else 'done')
    rows.append(f'| `{name}` | {title} | {m if staged else "-"} | {d} | {sk} | {state} |')
    tot_m+=m; tot_p+=d
    sec=re.search(r'^## Parked[^\n]*\n(.*?)(?=^## |\Z)',notes,re.S|re.M)
    if sec and d:
        parked.append(f'\n#### `{name}` ({d} parked; sources in `{p}/differ/`)\n'+sec.group(1).strip()+'\n')
gen=['| pending batch | chunk | matched (to land) | parked | skipped | state |','|---|---|---|---|---|---|']+rows
gen.append(f'\nTotals: **{tot_m} matched rows staged for landing**, **{tot_p} parked rows** for the Fable retry pass.\n')
skipped=[]
for name in sorted(os.listdir(root)) if os.path.isdir(root) else []:
    st=f'{root}/{name}/STATUS.tsv'
    if os.path.exists(st):
        for r in csv.DictReader(open(st),delimiter='\t'):
            if (r.get('status') or '').strip().upper()=='SKIP':
                skipped.append(f"| `{r['address']}` | `{name}` | {r.get('bytes','')} | {(r.get('note') or '').strip()} |")
gen.append('### Skipped rows (the row holds several functions: split it into one row per function, then author the pieces)')
gen+=(['| address | batch | bytes | note |','|---|---|---|---|']+skipped) if skipped else ['(none)']
gen.append('Also skipped before wave 25: `0073a840` (chunk 59: 0x0073A840 plus the 18-byte 0x0073A880). The chunk cutter never re-offers a used row, so these stay uncovered until split.')
gen.append('')
gen.append('### Parked rows per batch (hand-over lines written by the authoring agents)')
gen+=parked or ['(none yet)']
t=open(doc).read()
a=t.index('<!-- queue:begin -->')+len('<!-- queue:begin -->'); b=t.index('<!-- queue:end -->')
open(doc,'w').write(t[:a]+'\n'+'\n'.join(gen)+'\n'+t[b:])
print(len(rows),'batches,',tot_m,'matched,',tot_p,'parked')
