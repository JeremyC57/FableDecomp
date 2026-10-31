#!/usr/bin/env python3
"""v2 (owner 2026-10-31): rows whose residue text reads as a compiler tie (register colouring, x87 operand order, scheduling,
scratch rotation) go to a second bucket worked after the fixable rows (wrong control flow, frame layout, inlining, names).
Rank the sweep's parked (differ) rows for chunk cutting: not landed, not reserved (Codex 50, section 6b), not name_review,
no asm residue; closest residue first (same-length first, then by residue bytes, then by attempts). Writes $S/w30/ranked.tsv."""
import csv,os,re,glob,json
S=os.environ['S']; R='/home/user/FableDecomp'; P=f'{R}/rebuild/candidates/manual/pending'
def A(a): return a.lower().replace('0x','').rjust(8,'0')
landed={A(m.group(1)) for m in re.finditer(r"Address = '([0-9a-fA-F]{8})'",open(f'{R}/rebuild/build_candidates.ps1',errors='ignore').read())}
res=set()
# Codex parked-50 reservation released 2026-10-31 (RETURNED.tsv); its 39 differs are in the pool
for r in csv.DictReader(open(f'{R}/rebuild/candidates/manual/pending/2026-10-31_chatgpt_pass10/RESERVED.tsv'),delimiter='\t'): res.add(A(r['address']))
for r in csv.DictReader(open(f'{S}/retry/sec6b/WORKLIST.tsv'),delimiter='\t'): res.add(A(r['address']))
for line in open(f'{R}/rebuild/candidates/manual/chunk_history.tsv'):
    f=line.split('\t')
    if f and f[0].isdigit() and int(f[0])>=604: res.add(A(f[1]))
nr={A(os.path.basename(f)[:8]) for f in glob.glob(f'{P}/*/name_review/*')}
name={}
for line in open(f'{R}/rebuild/manifest/functions.tsv',encoding='utf-8-sig'):
    f=line.rstrip('\n').split('\t')
    if len(f)>2 and len(f[0])==8: name[f[0].lower()]=(f[1],f[2])
rows={}
for r in csv.DictReader(open(f'{S}/retry1/WORKLIST.tsv'),delimiter='\t'): rows[A(r['address'])]=r
# newest residue from wave-2 STATUS files (bytes "NNNvMMM" in residue/lever text)
stat={}
for st in sorted(glob.glob(f'{R}/rebuild/candidates/manual/batches/2026-10-3*_parked_retry*/STATUS.tsv')):
    for line in open(st,errors='ignore'):
        f=line.rstrip('\n').split('\t')
        if len(f)>=2 and re.fullmatch(r'[0-9a-f]{8}',f[0].lower()): stat[f[0].lower()]=(st.split('/')[-2],f)
TIE=re.compile(r'colou?ring|regalloc|x87|operand order|fmul|commuted|schedul|scratch|rotation|esi/edi|edi/esi|ebx/edi|edi/ebx|ebp/ebx|'
                 r'ebx/ebp|edx/ebp|ebp/edi|eax/ecx|ecx/eax|ecx/edx|edx/ecx|register swap|swapped|register pair|sib order|key-address|'
                 r'one register|single register|register choice|zero[- ]register|\bslot\b|spill|dead region|ltcg')
def residue_text(a,best):
    t=' '.join(stat.get(a,('',[]))[1])
    try:
        ls=open(f'{R}/{best}',errors='ignore').read().splitlines()[:4]
        t+=' '+' '.join(l for l in ls if l.startswith('//'))
    except Exception: pass
    return t.lower()
out=[]
for a,r in rows.items():
    if a in landed or a in res or a in nr: continue
    txt=' '.join(stat.get(a,('',[]))[1]).lower()
    if re.search(r'fistp|rdtsc|pushad|\bsse\b|mmx|xmm|retail_asm|inline asm|inline-asm|asm oracle',txt): continue
    rl=r['retail_len'] if r['retail_len'].isdigit() else ''
    resid=int(r['residue_bytes']) if r['residue_bytes'].isdigit() else 9999
    m=re.search(r'(\d+)v(\d+)',txt)
    if m: resid=abs(int(m.group(1))-int(m.group(2))); rl=m.group(2)
    best=r['best_source']
    newer=sorted(glob.glob(f'{P}/2026-10-3*/differ/{a}.cpp'))
    if newer: best=os.path.relpath(newer[-1],R)
    att=len(glob.glob(f'{P}/*/differ/{a}*.cpp'))
    rt=residue_text(a,best); tie=1 if TIE.search(rt) else 0
    out.append(dict(address=a,tie=tie,name=name.get(a,('?','?'))[0],module=name.get(a,('?','?'))[1],retail_len=rl,residue=resid,attempts=att,best=best,wave2=stat.get(a,('',))[0],range=r['range']))
out.sort(key=lambda c:(c['tie'],c['residue'],c['attempts'],int(c['retail_len'] or 0)))
with open(f'{S}/w30/ranked.tsv','w') as f:
    w=csv.DictWriter(f,list(out[0].keys()),delimiter='\t',lineterminator='\n'); w.writeheader(); w.writerows(out)
print(len(out),'rows ranked; fixable bucket:',sum(1 for c in out if c['tie']==0),' tie bucket:',sum(1 for c in out if c['tie']==1),'; residue 0:',sum(1 for c in out if c['residue']==0),' <=4:',sum(1 for c in out if c['residue']<=4))
for c in out[:30]: print(c['address'],'tie' if c['tie'] else 'fix',c['residue'],c['retail_len'],c['attempts'],c['range'],c['name'][:60])
