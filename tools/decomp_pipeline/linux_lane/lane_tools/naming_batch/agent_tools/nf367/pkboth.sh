#!/bin/bash
# pkboth.sh <tag> <table.tsv> <addr...>: both pick-any checkers (pickany_vs_retail.py, pk_all.py --row-length) on the HEAD (work/obj/H_<a>.obj)
# and new (work/obj/<a>.obj) objects of each row -> evidence/pk_<tag>.txt. NF348.
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; A=$S/w22/agentNF367; R=/home/user/FableDecomp
source $S/wine/env.sh >/dev/null 2>&1; unset TMPDIR TMP
tag=$1; tab=$2; shift 2; out=$A/evidence/pk_$tag.txt; : > $out
export FABLE_ROOT=$R FABLE_EXE="$S/dl/game/Fable The Lost Chapters/Fable.exe" PYTHONDONTWRITEBYTECODE=1
for a in "$@"; do
  len=$(awk -F'\t' -v a=$a '$1""==a""{print $3; exit}' $R/rebuild/oracles/auto-re-candidates.tsv)
  for k in H_ ""; do
    o=$A/work/obj/$k$a.obj; [ -f $o ] || { echo "## $a ${k:-new} MISSING $o" >> $out; continue; }
    echo "## $a ${k:-new} pickany_vs_retail (table $tab)" >> $out
    python3 -I $R/rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py "$FABLE_EXE" $tab $o $a >> $out 2>&1
    echo "## $a ${k:-new} pk_all --row-length=$len" >> $out
    python3 -I -B $R/tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py $o $a --row-length=$len >> $out 2>&1
  done
done
echo PK_DONE >> $out
