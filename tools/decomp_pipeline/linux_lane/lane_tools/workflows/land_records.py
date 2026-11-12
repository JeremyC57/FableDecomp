"""land_records.py <wf> <letter> <landed addrs comma> [<not-landed addrs comma>]: write the batch + pending record skeleton for a
workflow landing (payload, logs, concatenated per-row notes, verdicts, labels). READMEs and queue lines are written by the coordinator."""
import json,os,shutil,re,sys
S=os.environ['S']; wf,letter=sys.argv[1:3]; rows=sys.argv[3].split(','); other=sys.argv[4].split(',') if len(sys.argv)>4 and sys.argv[4] else []
Q=f'{S}/land/{wf}'; WF=f'{S}/w22/{wf}'
B=f'rebuild/candidates/manual/batches/2026-11-12_landing_{letter}'; P=f'rebuild/candidates/manual/pending/2026-11-12_linux_hand{wf.upper()}'
os.makedirs(B+'/logs',exist_ok=True); os.makedirs(P+'/hdr',exist_ok=True)
shutil.copy(Q+'/payload.json',B+'/payload.json'); shutil.copy(Q+'/land.log',B+'/logs/land.log')
res={r['addr']:r for r in json.load(open(WF+'/RESULT.json'))}
pay={r['address']:r for r in json.load(open(Q+'/payload.json'))['result']['authored']}
def cat(name,out,hdr='## %s\n\n'):
    t=''
    for a in rows+other:
        p=f'{WF}/{a}/{name}'
        if os.path.exists(p):
            body=open(p,errors='replace').read().rstrip('\n')
            if body: t+=(hdr%('0x'+a.upper()+(' (not landed)' if a in other else ''))) + body+'\n\n'
    open(out,'w').write(t)
cat('NOTES.md',P+'/NOTES.md'); cat('LESSONS.md',P+'/LESSONS.md'); cat('ATTEMPTS.log',P+'/ATTEMPTS.log','==== %s\n')
cat('hdr/README.md',P+'/hdr/README.md'); cat('DOSSIER.md',P+'/DOSSIERS.md')
for name in ('LABELS.tsv','NAME_FIXES.tsv','STATUS.tsv'):
    t=''
    for a in rows+other:
        p=f'{WF}/{a}/{name}'
        if os.path.exists(p):
            for l in open(p,errors='replace').read().split('\n'):
                if l.strip() and not l.lower().startswith('address\t'): t+=l+'\n'
    open(P+'/'+name,'w').write(t)
t=f'# Workflow {wf} verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)\n\n'
for a in rows+other:
    r=res[a]
    t+=f"## {a} -- {r['status']}, author attempts {r['attempts']}\n\n- What made it match: {r['what_made_it_match']}\n"
    for o in r['open_issues']: t+=f"- OPEN at hand-back: {o}\n"
    t+='\n'
open(P+'/EVIDENCE_VERDICTS.md','w').write(t)
with open(f'ghidra_out/labels_landing_2026-11-12{letter}.tsv','w') as f:
    f.write('address\tname\tevidence\n')
    for a in rows:
        m=re.search(r'Leaf:\s*(\S+)',pay[a]['source_cpp'])
        f.write(f"{a.upper()}\t{m.group(1) if m else pay[a]['module']+'::'+pay[a]['name']}\tworkflow {wf} (landing {letter}): RELOCATION_MATCH, two PDBs\n")
import subprocess
print(subprocess.run("grep -rEc '[0-9a-fA-F]{48,}' %s | grep -v ':0$'"%P,shell=True,capture_output=True,text=True).stdout or 'no long hex runs')
print('records',B,P)
