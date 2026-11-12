#!/bin/bash
# land_prep.sh <wf> <base-commit> addr... : reserved check, combine payloads, scans, header merge into $S/land/<wf>/M, hdrcheck2. Never fails.
wf=$1; base=$2; shift 2; L=tools/decomp_pipeline/linux_lane/lane_tools; Q=$S/land/$wf; M=$Q/M
mkdir -p $Q; rm -rf "${M:?}"; mkdir -p $M
for a in "$@"; do python3 $L/coordinator/reserved_check.py $S/w22/$wf/$a/payload.json 2>&1 | grep -v 'reserved lists\|mention a reserved address.*: 0$' | sed "s/^/[$a] /"; done
python3 $S/tools/combine_wf.py $S/w22/$wf $Q "$@"
python3 - $Q <<'P'
import json,re,os,sys
Q=sys.argv[1]; d=json.load(open(Q+'/payload.json'))
def strip(s):
    s=re.sub(r'/\*.*?\*/','',s,flags=re.S); return re.sub(r'//[^\n]*','',s)
for r in d['result']['authored']:
    a=r['address']
    for key in ('source_cpp','test_cpp'):
        c=strip(r[key])
        toks=('__asm','naked','_emit','register ','__forceinline','volatile','novtable','#pragma','__declspec(noinline)')
        bad=[t for t in toks if t in c]
        if bad and key=='source_cpp' or [t for t in bad if t in ('__asm','naked','_emit','register ','volatile')]:
            print(a,key,'TOKENS',bad,[l.strip()[:140] for l in c.split('\n') if any(t in l for t in bad)][:8])
        if key=='source_cpp':
            for m in re.finditer(r'^\s*(?:class|struct)\s+(?:__declspec\(\w+\)\s+)?(\w+)\s*(?::[^{;]*)?\{',c,re.M):
                if os.path.exists('rebuild/include/engine/%s.h'%m.group(1)): print(a,'LOCAL MODEL OF HEADER CLASS',m.group(1))
    if re.search('stand-in',r['source_cpp'],re.I): print(a,'STAND-IN text:',[l[:160] for l in r['source_cpp'].split('\n') if re.search('stand-in',l,re.I)][:4])
    for l in r['source_cpp'].split('\n'):
        if re.search(r'Inferred:|UNPROVEN|unproven',l): print(a,'  ',l[:220])
    if not re.search(r'^// Leaf: ',r['source_cpp'],re.M): print(a,'NO Leaf line')
P
for a in "$@"; do W=$S/w22/$wf/$a; [ -d $W/hdr ] || continue; (cd $W/hdr; find . -type f ! -name README.md | sed 's|^\./||') | while read h; do
  mkdir -p $M/$(dirname $h); cur=rebuild/include/$h; [ -f $M/$h ] && cur=$M/$h && echo "[$a] $h also changed by an earlier row: merging on top"
  if [ -f rebuild/include/$h ]; then
    if cmp -s $W/hdr/$h rebuild/include/$h; then continue; fi
    if [ "$(diff $cur $W/hdr/$h | grep -c '^<')" = 0 ]; then cp $W/hdr/$h $Q/_m.h; how=superset
    else git show $base:rebuild/include/$h > $Q/_base.h 2>/dev/null || cp rebuild/include/$h $Q/_base.h
      git merge-file -p -L repo -L base -L x $cur $Q/_base.h $W/hdr/$h > $Q/_m.h 2>/dev/null; how="merge_rc=$?"; fi
    cp $Q/_m.h $M/$h
    echo "[$a] $h $how removed=$(diff rebuild/include/$h $M/$h | grep -c '^<') added=$(diff rebuild/include/$h $M/$h | grep -c '^>') markers=$(grep -c '^<<<<<<<\|^>>>>>>>' $M/$h)"
  else cp $W/hdr/$h $M/$h; echo "[$a] $h NEW"; fi; done; done
python3 $L/coordinator/hdrcheck2.py $M 2>&1 | tr ' ' '\n' | grep -v '=same' | paste -sd' ' | sed 's/CHANGED/\nCHANGED/g'
echo; for m in $(python3 $L/coordinator/hdrcheck2.py $M 2>&1 | tr ' ' '\n' | grep '=DIFF' | sed 's/=DIFF.*//' | sort -u); do echo "$m users=$(grep -rlw $m rebuild/src rebuild/tests rebuild/include | wc -l)"; done
true
