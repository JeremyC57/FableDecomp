# usage (repo root): python3 records.py N  -- copy agentN records into the batch, pending differ, proven labels; print coverage
import os,re,sys,glob,shutil,csv
S=os.environ['S']; n=sys.argv[1]; A=f'{S}/w22/agent{n}'
B=f'rebuild/candidates/manual/batches/2026-10-27_linux_hand{n}'
for f in ('NOTES.md','LESSONS.md','STATUS.tsv'): shutil.copy(f'{A}/{f}',B)
open(f'{B}/verify.log','w').write(open(f'{S}/lv{n}.land.log').read()+((('=== retail_asm payload ===\n')+open(f'{S}/lv{n}r.land.log').read()) if os.path.exists(f'{S}/lv{n}r.land.log') else ''))
st=[l.rstrip('\n').split('\t') for l in open(f'{A}/STATUS.tsv')][1:]
dif=[p for p in st if len(p)>1 and (p[1]=='DIFFER' or os.path.exists(f'{A}/differ/{p[0]}.cpp'))]
if dif:
    P=f'rebuild/candidates/manual/pending/2026-10-27_linux_hand{n}'; os.makedirs(f'{P}/differ',exist_ok=True)
    for f in glob.glob(f'{A}/differ/*'): shutil.copy(f,f'{P}/differ/')
    open(f'{P}/STATUS.tsv','w').write(open(f'{A}/STATUS.tsv').readline()+''.join('\t'.join(p)+'\n' for p in dif))
note={p[0]:(p[4] if len(p)>4 else '') for p in st}
import re as _r
won={m.group(1):note.get(m.group(1),'') for m in _r.finditer(r'^([0-9a-f]{8}) +(?:RELOCATION_)?MATCH +PASS',(open(f'{S}/lv{n}.land.log').read()+(open(f'{S}/lv{n}r.land.log').read() if os.path.exists(f'{S}/lv{n}r.land.log') else '')),_r.M)}
L=open(f'{A}/NOTES.md').read().splitlines(); prov=set()
for i,l in enumerate(L):
    if re.match(r'^(#+\s*|-\s*|\*\*)?Proven',l.strip(),re.I):
        for m in L[i+1:]:
            if re.match(r'^(#+\s*|-\s*|\*\*)?(Probable|Descriptive)',m.strip(),re.I) or (m.startswith('#') and 'roven' not in m): break
            prov|=set(re.findall(r'(?<![0-9a-f])00[0-9a-f]{6}(?![0-9a-f])',m.lower()))
        break
if not prov: prov={a for a,v in won.items() if 'proven' in v and 'unproven' not in v.replace('CUnproven','')}
out=['address\tname\tevidence']
for a in sorted(prov&set(won)):
    nm=re.split(r' \(|; ',won[a])[0] or a; out.append(f"{a}\t{nm}\tproven per batch 2026-10-27_linux_hand{n} NOTES.md (chunk {n})")
open(f'ghidra_out/labels_chunk{n}_2026-10-27.tsv','w').write('\n'.join(out)+'\n')
print('won',len(won),'differ',len(dif),'proven labels',len(out)-1)
cs={l.split('\t')[0].lower() for l in open('rebuild/oracles/auto-re-candidates.tsv') if not l.startswith('address')}
rows=list(csv.DictReader(open('rebuild/manifest/functions.tsv'),delimiter='\t'))
z=lambda a:a.lower().replace('0x','').zfill(8)
allf={z(r['address']) for r in rows}; orig={z(r['address']) for r in rows if r['agent_source'] not in('gapscan','boundary')}
a=len(cs&allf);o=len(cs&orig);print(f'COVERAGE rows={len(cs)} all={a} ({100*a/len(allf):.1f}%) orig={o} ({100*o/len(orig):.1f}%) left={len(allf)-a}')
