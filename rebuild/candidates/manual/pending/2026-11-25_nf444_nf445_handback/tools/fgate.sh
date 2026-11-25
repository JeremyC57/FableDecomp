#!/bin/bash
# fgate.sh (NF445): HEAD v new for every row of rows.txt. H = the repo source (HEAD headers: overlay dir with no ../src), N = rows/<a>.cpp
# over $W/hdr with the payload flags. Each: q.py compile (object kept), pickany_vs_retail, pk_all (--funclet for 0121xxxx rows), and for
# N also pk_all --addrs=evidence/addrs_batch.tsv (only the names this batch binds). Output: evidence/pk/{H,N}_<a>.*; summary on stdout.
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; export S
source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
W=$S/w22/agentNF445; B=$W/batches/nf444_hdr; R=/home/user/FableDecomp; cd $R; o=$B/evidence/pk; mkdir -p $o $W/h_overlay/inc
EXE="$S/dl/game/Fable The Lost Chapters/Fable.exe"
for a in $(cat $B/rows.txt); do
  meta=$(python3 -c "
import json,sys,re
d=json.load(open('$B/head/payload.json'))['result']['authored']; e=[x for x in d if x['address']=='$a'][0]
print(' '.join(e.get('flags') or []) or '-'); print(e['leaf'])
" ); fl=$(echo "$meta" | sed -n 1p); leaf=$(echo "$meta" | sed -n 2p); [ "$fl" = "-" ] && fl=""
  hsrc=$(grep -n -A3 "Address = '$a'" rebuild/build_candidates.ps1 | grep -o "Source = '[^']*'" | head -1 | sed "s/Source = '//;s/'//")
  hfl=$(grep -n -A5 "Address = '$a'" rebuild/build_candidates.ps1 | grep -o "CompilerFlags = '[^']*'" | head -1 | sed "s/CompilerFlags = '//;s/'//;s#/O2##;s#/Oy##;s#/W3##")
  hleaf=$(awk -F'\t' -v a=$a '$1""==a""{print $3}' $B/RESULTS.tsv | head -1); [ -z "$hleaf" ] && hleaf=$leaf
  fx=(); pleaf="$leaf"; case $a in 0121*) fx=(--funclet); pleaf='?CreateRenderer@CSalSw3DVoice@@AAEPAVCSalRender@@W4CHRTFMODE@@@Z'; hpleaf='?CreateProcessor@CAudioSource@@QAEPAVCVtable_012beda4@@K@Z';; *) hpleaf="$hleaf";; esac
  len=$(awk -F'\t' -v a=$a '$1""==a""{print $3}' rebuild/oracles/auto-re-candidates.tsv | head -1)
  for tag in H N; do
    if [ $tag = H ]; then export FABLE_HDR_OVERLAY=$W/h_overlay/inc; src=rebuild/src/compiled/$hsrc; f=($hfl); L="$hpleaf"; else export FABLE_HDR_OVERLAY=$W/hdr; src=$B/rows/$a.cpp; f=($fl); L="$pleaf"; fi
    ff=(); [ ${#f[@]} -gt 0 ] && ff=(--flags "${f[@]}")
    python3 $S/tools/q.py $a "$src" "$L" "${ff[@]}" > $o/${tag}_$a.grade.txt 2>&1
    rm -f $o/${tag}_$a.obj; cp $S/wine/q/$a/$a.obj $o/${tag}_$a.obj 2>/dev/null || { echo "$a $tag NO_OBJ"; continue; }
    python3 -I rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py "$EXE" rebuild/oracles/reloc_bindings.tsv $o/${tag}_$a.obj $a > $o/${tag}_$a.pvr.txt 2>&1
    FABLE_EXE="$EXE" FABLE_ROOT=$R python3 -I -B tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py $o/${tag}_$a.obj $a --leaf="$L" --row-length=$len --tsv "${fx[@]}" > $o/${tag}_$a.pkall.tsv 2>&1
    [ $tag = N ] && FABLE_EXE="$EXE" FABLE_ROOT=$R python3 -I -B tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py $o/N_$a.obj $a --leaf="$L" --row-length=$len --tsv "${fx[@]}" --addrs=$B/evidence/addrs_batch.tsv > $o/N_$a.pkall_addrs.tsv 2>&1
    g=$(grep -m1 "^$a " $o/${tag}_$a.grade.txt | grep -o "RELOCATION_MATCH\|MATCH\|DIFFER([0-9v]*)\|SRC_FAIL\|LEAF[A-Z_]*" | head -1)
    ex=""; [ $tag = N ] && ex=" addrsNEQ=$(awk -F'\t' '$1 ~ /NEQ/' $o/N_$a.pkall_addrs.tsv | wc -l) addrsEQ=$(awk -F'\t' '$1=="EQ"' $o/N_$a.pkall_addrs.tsv | wc -l)"
    echo "$tag $a grade=${g:-?} flags=[${f[*]}] pvrNEQ=$(grep -c NEQ $o/${tag}_$a.pvr.txt) pkNEQ=$(awk -F'\t' '$1 ~ /NEQ/' $o/${tag}_$a.pkall.tsv | wc -l) pklines=$(grep -vc '^#' $o/${tag}_$a.pkall.tsv)$ex"
  done
done
echo FGATE_DONE
