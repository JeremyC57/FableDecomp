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
led='rebuild/candidates/manual/SKIPPED_ROWS.tsv'
lr=list(csv.DictReader(open(led),delimiter='\t')) if os.path.exists(led) else []
openr=[r for r in lr if r['status']!='done']
gen.append(f'### Skipped rows ({len(openr)} open of {len(lr)} in the ledger `{led}`)')
gen.append('Rows an agent could not author as one function. Work them with `skipped_ledger.py to-chunk <NN>` (multi/prefix pieces become an ordinary chunk for any agent); see the "Skipped rows" section above.')
gen+=(['| address | kind | status | pieces (addr:len) | from | note |','|---|---|---|---|---|---|']+[f"| `{r['address']}` | {r['kind']} | {r['status']} | {r['pieces'].replace(',',' ')} | `{r['batch']}` | {r['note'][:140]} |" for r in openr]) if openr else ['(none open)']
gen.append('')
gen.append('### Parked rows per batch (hand-over lines written by the authoring agents)')
gen+=parked or ['(none yet)']
t=open(doc).read()
a=t.index('<!-- queue:begin -->')+len('<!-- queue:begin -->'); b=t.index('<!-- queue:end -->')
open(doc,'w').write(t[:a]+'\n'+'\n'.join(gen)+'\n'+t[b:])
print(len(rows),'batches,',tot_m,'matched,',tot_p,'parked')
