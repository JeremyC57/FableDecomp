#!/bin/bash
# compose every w4a family onto fresh copies of the CURRENT repo files: compose_all.sh <out_dir>
# order: opeqw, opaddw (rule transforms), then the exact-block families. Prints every note/edited line.
W=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/naming/w4a
R=/home/user/FableDecomp; O=$(readlink -f "$1"); rm -rf "$O"; mkdir -p "$O"
FAMS="opeqw opaddw getas opconst sprintf filescan compare ctor zcdrop singles"
for f in $FAMS; do cat $W/fam_$f.files.txt; done | sort -u > $O.files.txt
while read p; do mkdir -p "$O/$(dirname $p)"; cp "$R/$p" "$O/$p"; done < $O.files.txt
for f in $FAMS; do
  args=$(sed "s|^|$O/|" $W/fam_$f.files.txt)
  echo "== $f"; (cd $R && python3 $W/fam_$f.py $args)
done
