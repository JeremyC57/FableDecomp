#!/bin/bash
# ngate2.sh (NF445): name_gate on every row of rows.txt, HEAD (repo source + test, HEAD headers, catalog flags) and new (rows/ + tests/, overlay,
# payload flags). Funclet rows are measured with the parent's leaf. Writes evidence/name_gate_{head,new}.txt; each row must end in a
# NAME_GATE summary line (a compile failure shows as a missing summary: reported as NO_SUMMARY).
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; export S
source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
W=$S/w22/agentNF445; B=$W/batches/nf444_hdr; R=/home/user/FableDecomp; cd $R
: > $B/evidence/name_gate_head.txt; : > $B/evidence/name_gate_new.txt
for a in $(cat $B/rows.txt); do
  meta=$(python3 -c "
import json
d=json.load(open('$B/head/payload.json'))['result']['authored']; e=[x for x in d if x['address']=='$a'][0]
print(' '.join(e.get('flags') or []) or '-'); print(e['leaf'])
open('$W/work/ngt_$a.cpp','w').write(e['test_cpp'])")
  fl=$(echo "$meta" | sed -n 1p); leaf=$(echo "$meta" | sed -n 2p); [ "$fl" = "-" ] && fl=""
  blk=$(grep -n -A5 "Address = '$a'" rebuild/build_candidates.ps1)
  hsrc=rebuild/src/compiled/$(echo "$blk" | grep -o " Source = '[^']*'" | head -1 | sed "s/ Source = '//;s/'//")
  htst=rebuild/tests/$(echo "$blk" | grep -o "TestSource = '[^']*'" | head -1 | sed "s/TestSource = '//;s/'//")
  hfl=$(echo "$blk" | grep -o "CompilerFlags = '[^']*'" | head -1 | sed "s/CompilerFlags = '//;s/'//;s#/O2##;s#/Oy##;s#/W3##")
  hleaf=$(awk -F'\t' -v a=$a 'NR>1 && $1""==a""{print $3}' $B/RESULTS.tsv | sed 's/ (parent)//')
  case $a in 0121*) leaf='?CreateRenderer@CSalSw3DVoice@@AAEPAVCSalRender@@W4CHRTFMODE@@@Z';; esac
  ht=(); [ -f "$htst" ] && ht=(--test "$htst"); hf=(); [ -n "$(echo $hfl)" ] && hf=(--flags $hfl); nf=(); [ -n "$fl" ] && nf=(--flags $fl)
  echo "## $a $hleaf" >> $B/evidence/name_gate_head.txt
  FABLE_HDR_OVERLAY=$W/h_overlay/inc timeout 300 python3 $S/tools/name_gate.py $a $hsrc "$hleaf" "${ht[@]}" "${hf[@]}" > $W/work/ng.txt 2>&1
  grep -E "^(FAIL|NAME_GATE|DEBT|DESC)" $W/work/ng.txt | cut -c1-260 >> $B/evidence/name_gate_head.txt; grep -q "^NAME_GATE" $W/work/ng.txt || echo "NO_SUMMARY $a H" >> $B/evidence/name_gate_head.txt
  echo "## $a $leaf" >> $B/evidence/name_gate_new.txt
  FABLE_HDR_OVERLAY=$W/hdr timeout 300 python3 $S/tools/name_gate.py $a $B/rows/$a.cpp "$leaf" --test $W/work/ngt_$a.cpp "${nf[@]}" > $W/work/ng.txt 2>&1
  grep -E "^(FAIL|NAME_GATE|DEBT|DESC)" $W/work/ng.txt | cut -c1-260 >> $B/evidence/name_gate_new.txt; grep -q "^NAME_GATE" $W/work/ng.txt || { echo "NO_SUMMARY $a N" >> $B/evidence/name_gate_new.txt; tail -3 $W/work/ng.txt >> $B/evidence/name_gate_new.txt; }
done
echo DONE >> $B/evidence/name_gate_new.txt
