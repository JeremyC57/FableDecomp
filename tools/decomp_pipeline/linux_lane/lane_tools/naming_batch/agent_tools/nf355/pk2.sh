#!/bin/bash
# pk2.sh <tag> <addr> <obj> <leaf> [table] : both pick-any checkers on one object with an explicit --leaf (GOTCHAS 2026-11-24) and the
# NF355 caller-given address file (work/pk_addrs.tsv) -> evidence/pk_<tag>.txt (appends). NF355.
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; A=$S/w22/agentNF355; R=/home/user/FableDecomp
source $S/wine/env.sh >/dev/null 2>&1; unset TMPDIR TMP
tag=$1; a=$2; o=$3; leaf=$4; tab=${5:-$R/rebuild/oracles/reloc_bindings.tsv}; out=$A/evidence/pk_$tag.txt
export FABLE_ROOT=$R FABLE_EXE="$S/dl/game/Fable The Lost Chapters/Fable.exe" PYTHONDONTWRITEBYTECODE=1
len=$(awk -F'\t' -v a=$a '$1==a{print $3; exit}' $R/rebuild/oracles/auto-re-candidates.tsv)
echo "## $a $o pickany_vs_retail --leaf=$leaf (table $tab)" >> $out
python3 -I $R/rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py "$FABLE_EXE" $tab $o $a --leaf=$leaf >> $out 2>&1
echo "## $a $o pk_all --leaf=$leaf --row-length=$len --addrs=$A/work/pk_addrs.tsv" >> $out
python3 -I -B $R/tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py $o $a --leaf=$leaf --row-length=$len --addrs=$A/work/pk_addrs.tsv >> $out 2>&1
echo "## done" >> $out
