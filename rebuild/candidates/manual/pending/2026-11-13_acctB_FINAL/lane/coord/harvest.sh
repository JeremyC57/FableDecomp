#!/bin/bash
# harvest.sh <folder-tag> <wf>/<addr> ... : coordinator re-verification (read-only vland, strict headers, row overlay, purity scan)
# of workflow rows and staging into rebuild/candidates/manual/pending/<date>_acctB_<tag>/ (no oracle bytes). Never lands.
export S=/opt/fable_lane; source $S/wine/env.sh >/dev/null 2>&1; export S; cd /home/user/FableDecomp
TAG=$1; shift; D=rebuild/candidates/manual/pending/$(date +%Y-%m-%d)_acctB_$TAG
mkdir -p $D/rows $D/hdr $D/notes $D/coordverify
[ -f $D/STATUS.tsv ] || printf 'address\tgrade\tbytes\ttest\tmodule\tname\tflags\tnotes\n' > $D/STATUS.tsv
for x in "$@"; do
  a=$(basename $x); W=$S/w22/$x; mkdir -p $W/coordverify
  python3 - $W $a <<'E'
import json,sys,re
W,a=sys.argv[1],sys.argv[2]; r=[c for c in json.load(open(W+'/payload.json'))['result']['authored'] if c['address']==a][0]
open(W+'/coordverify/src.cpp','w').write(r['source_cpp']); open(W+'/coordverify/test.cpp','w').write(r['test_cpp'])
open(W+'/coordverify/meta.txt','w').write(r['module']+'\t'+r['name']+'\t'+' '.join(r.get('flags',[]))+'\n')
strip=lambda s: re.sub(r'//[^\n]*','',s)
bad=[w for w in ('__asm','_emit','naked','register','__forceinline','volatile') if re.search(r'\b'+w+r'\b',strip(r['source_cpp'])+strip(r['test_cpp']))]
open(W+'/coordverify/purity.txt','w').write(' '.join(bad) or 'clean')
E
  export FABLE_HDR_OVERLAY=$W/hdr; rm -rf $W/lv_coord
  LANDVERIFY_WORK=$W/lv_coord LANDVERIFY_STRICT_HEADERS=1 timeout 900 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $W/payload.json $W/oracle.tsv > $W/coordverify/vland.log 2>&1
  rm -rf $W/lv_coord
  line=$(grep -E "^$a +(RELOCATION_MATCH|MATCH)(\[[^]]*\])? +PASS" $W/coordverify/vland.log | tail -1)
  pur=$(cat $W/coordverify/purity.txt); stale=$(grep -c STALE $W/hdr/README.md 2>/dev/null)
  echo "$x | ${line:-NOT VERIFIED: $(grep -E "^$a " $W/coordverify/vland.log | tail -1 | cut -c1-90)} | purity: $pur | warn $(grep -c WARN $W/coordverify/vland.log) conflicts $(grep -c -E 'RELOC_CONFLICT|RELOC_IDENTITY|NOT_GENUINE' $W/coordverify/vland.log) stale-hdr ${stale:-0}"
  [ -z "$line" ] && continue
  [ "$pur" != clean ] && echo "   (purity words found: staged anyway, listed in STATUS)"
  cp $W/coordverify/src.cpp $D/rows/$a.cpp; cp $W/coordverify/test.cpp $D/rows/$a.test.cpp; cp $W/coordverify/vland.log $D/coordverify/$a.vland.log
  mkdir -p $D/notes/$a; for f in NOTES.md DOSSIER.md OUTLINE.md ATTEMPTS.log LESSONS.md NAME_FIXES.tsv LABELS.tsv; do [ -f $W/$f ] && cp $W/$f $D/notes/$a/; done
  rm -rf $D/hdr/$a; if [ -n "$(find $W/hdr -name '*.h' 2>/dev/null | head -1)" ]; then mkdir -p $D/hdr/$a; cp -r $W/hdr/. $D/hdr/$a/; fi
  g=$(echo $line | awk '{print $2}' | sed "s/\[.*//"); len=$(grep "^$a" $W/oracle.tsv | cut -f3)
  grep -v "^$a" $D/STATUS.tsv > $D/STATUS.tmp; mv $D/STATUS.tmp $D/STATUS.tsv
  printf '%s\t%s\t%s\tPASS (VC7.1, lander check)\t%s\t%s\t%s\tpurity %s; %s stale overlay header(s)\n' $a $g $len "$(cut -f1 $W/coordverify/meta.txt)" "$(cut -f2 $W/coordverify/meta.txt)" "$(cut -f3 $W/coordverify/meta.txt)" "$pur" "${stale:-0}" >> $D/STATUS.tsv
done
python3 - $D <<'E'
import json,sys,csv,glob,os
D=sys.argv[1]; out={'result':{'authored':[]}}
for r in csv.DictReader(open(D+'/STATUS.tsv'),delimiter='\t'):
    a=r['address']
    for p in glob.glob('/opt/fable_lane/w22/bwf*/'+a+'/payload.json'):
        out['result']['authored']+=[c for c in json.load(open(p))['result']['authored'] if c['address']==a]
json.dump(out,open(D+'/payload.json','w'),indent=1); print('payload rows',len(out['result']['authored']))
E
echo "staged in $D; oracle files: $(find $D -name 'oracle.tsv' | wc -l)"
true
