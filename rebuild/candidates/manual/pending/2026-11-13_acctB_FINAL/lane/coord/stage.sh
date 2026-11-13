#!/bin/bash
# usage: stage.sh <N> <name>  -- copy agent<N>'s verified hand-back into rebuild/candidates/manual/pending/<date>_acctB_<name>/ (no landing, no oracle bytes)
S=/opt/fable_lane; N=$1; A=$S/w22/agent$N
D=/home/user/FableDecomp/rebuild/candidates/manual/pending/$(date +%Y-%m-%d)_acctB_$2
mkdir -p $D/rows $D/hdr
for f in $A/src/*.cpp; do [ -e "$f" ] || continue; a=$(basename $f .cpp); cp $f $D/rows/$a.cpp; cp $A/test/$a.cpp $D/rows/$a.test.cpp; done
if ls $A/differ/* >/dev/null 2>&1; then mkdir -p $D/rows/parked; cp $A/differ/* $D/rows/parked/; fi
[ -d $A/hdr ] && cp -r $A/hdr/. $D/hdr/
for f in STATUS.tsv NAME_FIXES.tsv LABELS.tsv LESSONS.md NOTES.md SPLITS.tsv payload.json; do [ -e $A/$f ] && cp $A/$f $D/; done
[ -d $A/coordverify ] && mkdir -p $D/coordverify && cp $A/coordverify/*.txt $A/coordverify/vland.log $D/coordverify/ 2>/dev/null
echo $D; find $D -type f | wc -l; du -sh $D
