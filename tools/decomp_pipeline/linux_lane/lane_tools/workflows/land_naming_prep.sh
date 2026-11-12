#!/bin/bash
# land_naming_prep.sh <agent-dir-name> <base-commit>: checks before a naming landing (overlap with landings since base, busy rows,
# new local models, purity tokens, header overlay = repo + additions, hdrcheck2, section-1 file for batch_rerow). Never fails.
ag=$1; base=$2; L=tools/decomp_pipeline/linux_lane/lane_tools; N=$S/w22/$ag; Q=$S/land/$ag; mkdir -p $Q
git diff --name-only $base HEAD -- rebuild/src/compiled rebuild/tests | sed -E 's/.*_([0-9a-f]{8})(_test)?\.cpp$/\1/' | sort -u > $Q/changed.txt
ls $N/rows | sed -E 's/^([0-9a-f]{8}).*/\1/' | sort -u > $Q/rows.txt; echo "rows $(wc -l < $Q/rows.txt) files $(ls $N/rows | wc -l)"; echo "overlap with landings since $base:"; comm -12 $Q/changed.txt $Q/rows.txt
for h in $(cd $N/hdr; find . -type f ! -name README.md | sed 's|^\./||'); do [ -f rebuild/include/$h ] && echo "$h removed_vs_HEAD=$(diff rebuild/include/$h $N/hdr/$h | grep -c '^<') added=$(diff rebuild/include/$h $N/hdr/$h | grep -c '^>')" || echo "$h NEW"; done
python3 $L/coordinator/hdrcheck2.py $N/hdr 2>&1 | tr ' ' '\n' | grep -v '=same' | paste -sd' ' | sed 's/CHANGED/\nCHANGED/g'; echo
for m in $(python3 $L/coordinator/hdrcheck2.py $N/hdr 2>&1 | tr ' ' '\n' | grep '=DIFF' | sed 's/=DIFF.*//' | sort -u); do echo "$m users=$(grep -rlw $m rebuild/src rebuild/tests rebuild/include | wc -l)"; done
python3 - $N $Q <<'P'
import os,re,glob,sys
N,Q=sys.argv[1:3]
bc=open('rebuild/build_candidates.ps1',encoding='utf-8-sig').read()
ent={}
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}",bc,re.S):
    b=m.group(1); a=re.search(r"Address = '([0-9a-fA-F]{8})'",b)
    if a: ent[a.group(1).lower()]=dict(re.findall(r"(\w+) = '([^']*)'",b))
def strip(s):
    s=re.sub(r'/\*.*?\*/','',s,flags=re.S); return re.sub(r'//[^\n]*','',s)
def models(s): return set(re.findall(r'^\s*(?:class|struct)\s+(?:__declspec\(\w+\)\s+)?(\w+)\s*(?::[^{;]*)?\{',strip(s),re.M))
busy=set(open(N+'/BUSY.txt').read().split())
for f in sorted(glob.glob(N+'/rows/*.cpp')):
    a=os.path.basename(f)[:8]; t='_test' in f
    if a in busy: print('BUSY ROW',a)
    d=ent.get(a)
    if not d: print('NOT LANDED',a); continue
    old=open(('rebuild/tests/'+d['TestSource']) if t else ('rebuild/src/compiled/'+d['Source']),errors='replace').read(); new=open(f,errors='replace').read()
    for c in models(new)-models(old):
        if os.path.exists(f'rebuild/include/engine/{c}.h'): print('NEWLOCAL',a,c,'(test)' if t else '')
    bad=[x for x in ('__asm','naked','_emit','register ','__forceinline','volatile') if x in strip(new) and x not in strip(old)]
    if bad: print('TOKENS',a,bad)
    if 'naked' in new and 'naked' not in old: print('WORD naked (purity scanner)',a)
ls=open(N+'/RENAMES.tsv').read().split('\n'); out=[]
for l in ls:
    if l.startswith('# Section 2'): break
    c=l.split('\t')
    if len(c)>2 and re.fullmatch(r"(0x)?[0-9a-fA-F]{8}",c[0].strip()):
        c[0]="0x"+c[0].strip().lower().replace("0x","")
        if not c[1].strip(): c[1]='_global'
        if '::' in c[2] and not c[2].startswith('operator'): print('NAME HAS ::',c[0],c[2])
    out.append('\t'.join(c))
open(Q+'/sec1.tsv','w').write('\n'.join(out).rstrip('\n')+'\n'); print('sec1 rows',len([o for o in out if o.lower().startswith('0x')]))
open(Q+'/skip.txt','w').write('')
P
true
