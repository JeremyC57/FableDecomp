#!/bin/bash
# meas.sh <addr> <src.cpp> <leaf> <rowlen> <tag> [flags...]: one q.py grade (overlay = auth30/hdr) + both pick-any checkers + vtchk.
# Writes <addr>/<tag>.out, <tag>.obj, <tag>.pk.txt, <tag>.pkall.tsv, <tag>.vt.txt and prints a short summary.
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S; unset TMPDIR TMP
cd /home/user/FableDecomp
a=$1; src=$2; leaf=$3; n=$4; tag=$5; shift 5
W=$S/w22/auth30; D=$W/$a; mkdir -p $D
EXE="$S/dl/game/Fable The Lost Chapters/Fable.exe"
rm -f $S/wine/q/$a/$a.obj
if [ $# -gt 0 ]; then FL=(--flags "$@"); else FL=(); fi
FABLE_HDR_OVERLAY=$W/hdr LANDVERIFY_STRICT_HEADERS=1 timeout 900 python3 $S/tools/q.py $a $src "$leaf" "${FL[@]}" > $D/$tag.out 2>&1
head -1 $D/$tag.out
[ -f $S/wine/q/$a/$a.obj ] || { echo NOOBJ; exit 0; }
cp $S/wine/q/$a/$a.obj $D/$tag.obj
python3 -I rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py "$EXE" rebuild/oracles/reloc_bindings.tsv $D/$tag.obj $a --leaf=$leaf > $D/$tag.pk.txt 2>&1
FABLE_EXE="$EXE" FABLE_ROOT=/home/user/FableDecomp python3 -I -B tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py $D/$tag.obj $a --leaf=$leaf --row-length=$n > $D/$tag.pkall.tsv 2>&1
python3 -I $W/tools/vtchk.py $D/$tag.obj > $D/$tag.vt.txt 2>&1
echo "-- pickany_vs_retail:"; grep -c '^EQ' $D/$tag.pk.txt | sed 's/^/EQ /'; grep '^NEQ' $D/$tag.pk.txt
echo "-- pk_all (non-NOADDR, non-EQ):"; head -1 $D/$tag.pkall.tsv; grep -v '^#' $D/$tag.pkall.tsv | grep -v '^NOADDR' | grep -v '^EQ ' | cut -c1-200
echo "-- pk_all EQ count: $(grep -c '^EQ ' $D/$tag.pkall.tsv)  NOADDR: $(grep -c '^NOADDR' $D/$tag.pkall.tsv)"
echo "-- vtchk:"; grep -E 'VFTABLE|retail 0x|WRONG|unknown|NEQ|SUMMARY' $D/$tag.vt.txt | cut -c1-220
