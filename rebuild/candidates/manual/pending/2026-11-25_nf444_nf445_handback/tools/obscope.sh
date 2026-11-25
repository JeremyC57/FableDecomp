#!/bin/bash
# obscope.sh: regrade a deterministic sample of 20 landed rows in 0x00C50000..0x00C7FFFF (outside this batch) with their catalog flags + /Ob1.
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; export S
source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
W=$S/w22/agentNF445; cd /home/user/FableDecomp; export FABLE_HDR_OVERLAY=$W/empty_overlay
out=$W/evidence/ob1_scope.txt; : > $out
python3 - > $W/evidence/ob1_sample.tsv <<'PY'
import re,random
bc=open('rebuild/build_candidates.ps1',encoding='utf-8',errors='replace').read()
ss={}
for l in open('rebuild/oracles/self_symbols.tsv',encoding='utf-8',errors='replace'):
    p=l.rstrip('\n').split('\t')
    if len(p)>2: ss[p[0]]=(p[1],p[2])
batch=set(open('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF445/batches/nf444_hdr/rows.txt').read().split())|{'00c55270','00c55680','00c556e0','00c6e660'}
rows=[]
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}",bc,re.S):
    d=dict(re.findall(r"(\w+) = '([^']*)'",m.group(1))); a=d.get('Address','').lower()
    if '00c50000'<=a<'00c80000' and a not in batch and a in ss and ss[a][1] in ('MATCH','RELOCATION_MATCH') and 'Source' in d:
        rows.append((a,d['Source'],ss[a][0],d.get('CompilerFlags','')))
random.seed(444); 
for r in sorted(random.sample(rows,min(20,len(rows)))): print('\t'.join(r))
PY
while IFS=$'\t' read a src leaf fl; do
  extra=$(echo "$fl" | sed 's#/O2##;s#/Oy##;s#/W3##')
  g=$(python3 $S/tools/q.py $a rebuild/src/compiled/$src "$leaf" --flags $extra /Ob1 2>&1 | grep -m1 "^$a " | grep -o "RELOCATION_MATCH\|MATCH\|DIFFER([0-9v]*)\|SRC_FAIL")
  echo -e "$a\t${g:-ERR}\t$extra /Ob1\t$src" >> $out
done < $W/evidence/ob1_sample.tsv
echo DONE >> $out
