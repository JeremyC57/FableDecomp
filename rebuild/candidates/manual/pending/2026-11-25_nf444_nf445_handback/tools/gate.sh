#!/bin/bash
# gate.sh <tag H|N> <addr> <leaf> <src.cpp> [flags...]: compile one row with the lane grader (q.py), keep its object, run
# pickany_vs_retail and pk_all on it. Outputs $W/evidence/pk/<tag>_<addr>.{obj,grade.txt,pvr.txt,pkall.tsv}.
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; export S
source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
W=$S/w22/agentNF445; R=/home/user/FableDecomp; cd $R
tag=$1; a=$2; leaf=$3; src=$4; shift 4
fl=(); [ $# -gt 0 ] && fl=(--flags "$@")
o=$W/evidence/pk; mkdir -p $o
[ "$tag" = N ] && export FABLE_HDR_OVERLAY=$W/hdr || export FABLE_HDR_OVERLAY=$W/h_overlay/inc
mkdir -p $W/empty_overlay
python3 $S/tools/q.py $a "$src" "$leaf" "${fl[@]}" > $o/${tag}_$a.grade.txt 2>&1
cp $S/wine/q/$a/$a.obj $o/${tag}_$a.obj 2>/dev/null || { echo "$a $tag NO_OBJ"; exit 0; }
EXE="$S/dl/game/Fable The Lost Chapters/Fable.exe"
len=$(awk -F'\t' -v a=$a '$1""==a""{print $3}' rebuild/oracles/auto-re-candidates.tsv | head -1)
python3 -I rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py "$EXE" rebuild/oracles/reloc_bindings.tsv $o/${tag}_$a.obj $a > $o/${tag}_$a.pvr.txt 2>&1
FABLE_EXE="$EXE" FABLE_ROOT=$R python3 -I -B tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py $o/${tag}_$a.obj $a --leaf="$leaf" --row-length=$len --tsv > $o/${tag}_$a.pkall.tsv 2>&1
g=$(grep -m1 "^$a " $o/${tag}_$a.grade.txt | cut -c1-80)
echo "$tag $a grade=[$g] pvr_NEQ=$(grep -c NEQ $o/${tag}_$a.pvr.txt) pk_NEQ=$(awk -F'\t' '$1 ~ /NEQ/' $o/${tag}_$a.pkall.tsv | wc -l) pk_lines=$(wc -l < $o/${tag}_$a.pkall.tsv)"
