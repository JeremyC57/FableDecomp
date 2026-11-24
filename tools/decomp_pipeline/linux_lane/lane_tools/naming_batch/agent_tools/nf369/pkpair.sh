#!/bin/bash
# pkpair.sh <dir> <H|N> <addr> <obj> <leaf>: both pick-any checkers on one object with an explicit leaf ->
#   <dir>/<H|N>_<addr>.pkall.tsv (pk_all --tsv) and <dir>/<H|N>_<addr>.pvr.txt (pickany_vs_retail). NF369 (from NF367).
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; R=/home/user/FableDecomp
source $S/wine/env.sh >/dev/null 2>&1; unset TMPDIR TMP
d=$1; k=$2; a=$3; o=$4; leaf=$5; mkdir -p $d
export FABLE_ROOT=$R FABLE_EXE="$S/dl/game/Fable The Lost Chapters/Fable.exe" PYTHONDONTWRITEBYTECODE=1
len=$(awk -F'\t' -v a=$a '$1""==a""{print $3; exit}' $R/rebuild/oracles/auto-re-candidates.tsv)
python3 -I -B $R/tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py $o $a --leaf=$leaf --row-length=$len --tsv > $d/${k}_$a.pkall.tsv 2>&1
python3 -I $R/rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py "$FABLE_EXE" $R/rebuild/oracles/reloc_bindings.tsv $o $a --leaf=$leaf > $d/${k}_$a.pvr.txt 2>&1
echo "len=$len" >> $d/${k}_$a.pvr.txt
# plus the caller-given address file (tools/pk_addrs.tsv: the ??_G / ??1 homes the tables lack) -> <dir>/<H|N>_<addr>.pkallo.tsv
python3 -I -B $R/tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py $o $a --leaf=$leaf --row-length=$len --tsv --addrs=$S/w22/agentNF369/tools/pk_addrs.tsv > $d/${k}_$a.pkallo.tsv 2>&1
